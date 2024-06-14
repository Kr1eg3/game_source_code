// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/Bullet.h"
#include "Ammo/BoxBaseComponent.h"
#include "Ammo/BulletCombatPartComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HitInterface.h"
#include "REnemy.h"
#include "GameFrameWork/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	bulletShell = CreateDefaultSubobject<UBoxBaseComponent>(TEXT("BulletShell"));
	auto collisionBox = bulletShell->getCollisionBoxPtr();
	SetRootComponent(collisionBox);

	bulletCombatPart = CreateDefaultSubobject<UBulletCombatPartComponent>(TEXT("BulletCombatPart"));

	bulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	bulletMovementComponent->bRotationFollowsVelocity = true;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	bulletShell->getCollisionBoxPtr()->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	auto tracer = bulletCombatPart->getTracerPtr();
	if (tracer)
	{
		auto tracerComponent = bulletCombatPart->getTracerComponentPtr();
		if (tracerComponent)
			tracerComponent = UGameplayStatics::SpawnEmitterAttached(
				tracer,
				bulletShell->getCollisionBoxPtr(),
				FName(),
				GetActorLocation(),
				GetActorRotation(),
				EAttachLocation::KeepWorldPosition);
	}
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
				auto damage = bulletCombatPart->getDamage();
				auto force = bulletCombatPart->getForce();
				UGameplayStatics::ApplyDamage(
					Hit.GetActor(),
					damage,
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

					goreSystem->ApplyGoreDamage(damage, true, force, hitForGore, NewBoneHP, DamageAplied);

					if (Hit.BoneName == "neck_01")
						enemy->ForceDie();


					//TArray<FGoreSystemHit> multDestroy;
					//std::unordered_map<int, FName> map = { {0, FName("upperarm_l")}, {1, FName("upperarm_r")}, {2, FName("neck_01")} };
					//for (int i = 0; i < 3; i++) {
					//	FGoreSystemHit tmp;
					//	tmp.BoneName = map[i];
					//	tmp.Location = Hit.ImpactPoint;
					//	tmp.Normal = Hit.ImpactNormal;
					//	multDestroy.Add(tmp);
					//}
					//goreSystem->DestroyBones(multDestroy);

				}
			}
		}
	}

	Destroy();
}

void ABullet::Destroyed()
{
	Super::Destroyed();

	auto& actorTransform = GetActorTransform();
	bulletCombatPart->spawnNiagaraImpactParticlesAtLocation(actorTransform.GetLocation(), actorTransform.Rotator());
	bulletCombatPart->playImpactSoundAtLocation(actorTransform.GetLocation());
}
