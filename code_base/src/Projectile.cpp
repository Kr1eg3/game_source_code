// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "HitInterface.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	m_CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(m_CollisionBox);
	m_CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	m_CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	m_CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	m_CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	m_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	m_ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (m_Tracer)
	{
		m_TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			m_Tracer,
			m_CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
			);
	}

	m_CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Hitted Actor: %f"), *OtherActor->GetName()));

	AREnemy* enemy = Cast<AREnemy>(OtherActor);
	if (enemy)
	{
		if (Hit.GetActor())
		{
			IHitInterface* hitInterface = Cast<IHitInterface>(Hit.GetActor());
			if (hitInterface)
			{
				hitInterface->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint);

				UGameplayStatics::ApplyDamage(
					Hit.GetActor(),
					m_Damage,
					GetInstigator()->GetController(),
					this,
					UDamageType::StaticClass()
				);

				auto goreSystem = enemy->getGoreSystem();
				if (goreSystem) {
					FGoreSystemHit hitForGore;
					hitForGore.BoneName = Hit.BoneName;
					hitForGore.Location = Hit.ImpactPoint;
					hitForGore.Normal = Hit.ImpactNormal;

					float NewBoneHP;
					float DamageAplied;

					goreSystem->ApplyGoreDamage(m_Damage, true, 1000.f, hitForGore, NewBoneHP, DamageAplied);
				}


			}
		}
	}

	Destroy();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	auto& actorTransform = GetActorTransform();

	if (m_nImpactParticles) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner(), m_nImpactParticles, actorTransform.GetLocation(), actorTransform.Rotator(), FVector(1.0f), true, true);
	}
	else {
		if (m_ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ImpactParticles, GetActorTransform());
		}
	}

	if (m_ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_ImpactSound, GetActorLocation());
	}
}

