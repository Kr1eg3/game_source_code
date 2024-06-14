// Fill out your copyright notice in the Description page of Project Settings.


#include "CyberRemains.h"


ACyberRemains::ACyberRemains()
{
	PrimaryActorTick.bCanEverTick = true;

	auto* Socket = this->GetMesh();

	// Collision spheres for Cyber Remains bot attack detection     (Collision Box Extend: X = 23.f, Y = 5.f, Z = 10.f)
	m_leftHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand"));
	m_leftHitBox->SetupAttachment(GetMesh(), "hand_l");
	m_leftHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_leftHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_leftHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//m_leftHand->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//m_leftHand->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	//
	m_rightHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand"));
	m_rightHitBox->SetupAttachment(GetMesh(), "hand_r");
	m_rightHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_rightHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_rightHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//m_rightHand->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//m_rightHand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//m_rightHand->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//m_rightHand->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//m_rightHand->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	//m_rightHand->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);


	m_leftBoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Left Box Trace Start"));
	m_leftBoxTraceStart->SetupAttachment(GetMesh(), "hand_l");
	m_leftBoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Left Box Trace End"));
	m_leftBoxTraceEnd->SetupAttachment(GetMesh(), "hand_l");

	m_rightBoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Right Box Trace Start"));
	m_rightBoxTraceStart->SetupAttachment(GetMesh(), "hand_r");
	m_rightBoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Right Box Trace End"));
	m_rightBoxTraceEnd->SetupAttachment(GetMesh(), "hand_r");


	//nEyesEffect = CreateDefaultSubobject<UNiagaraSystem>(TEXT("Eyes Effect"));

	leftEyeLocation = { 0, 0, 0 };
}

void ACyberRemains::SetLeftHitBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	GetLeftHitBox()->SetCollisionEnabled(CollisionEnabled);
}

void ACyberRemains::SetRightHitBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	GetRightHitBox()->SetCollisionEnabled(CollisionEnabled);
}

void ACyberRemains::BeginPlay()
{
	Super::BeginPlay();

	m_leftHitBox->OnComponentBeginOverlap.AddDynamic (this, &ACyberRemains::OnBoxOverlap);
	m_rightHitBox->OnComponentBeginOverlap.AddDynamic(this, &ACyberRemains::OnBoxOverlap);
	//m_rightHand->OnComponentHit.AddDynamic(this, &ACyberRemains::OnHit);


	auto leftEyeEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(nEyesEffect, GetMesh(), "neck_01",
		leftEyeLocation, FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);


}

//bool ACyberRemains::ActorIsSameType(AActor* OtherActor)
//{
//	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
//}

void ACyberRemains::BoxTrace(FHitResult& BoxHit)
{
	if (m_bLeftHandAttack)
	{
		const FVector Start = m_leftBoxTraceStart->GetComponentLocation();
		const FVector End = m_leftBoxTraceEnd->GetComponentLocation();

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		ActorsToIgnore.Add(GetOwner());

		for (AActor* Actor : m_IgnoreActors)
		{
			ActorsToIgnore.AddUnique(Actor);
		}

		UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			m_BoxTraceExtent,
			m_leftBoxTraceStart->GetComponentRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			m_bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			BoxHit,
			true
		);

	}
	if (m_bRightHandAttack)
	{
		const FVector Start = m_rightBoxTraceStart->GetComponentLocation();
		const FVector End = m_rightBoxTraceEnd->GetComponentLocation();

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		ActorsToIgnore.Add(GetOwner());

		for (AActor* Actor : m_IgnoreActors)
		{
			ActorsToIgnore.AddUnique(Actor);
		}

		UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			m_BoxTraceExtent,
			m_rightBoxTraceStart->GetComponentRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			m_bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			BoxHit,
			true
		);
	}

	//m_IgnoreActors.AddUnique(BoxHit.GetActor());
}

void ACyberRemains::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
	}
}

void ACyberRemains::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ARChar* enemy = Cast<ARChar>(OtherActor);
	if (enemy)
	{
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Char get hitted!")));
		if (Hit.GetActor() && Hit.GetActor()->ActorHasTag(FName("V9")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("Hitted Actor: %s"), *Hit.GetActor()->GetName()));


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
			}
		}
	}
}

void ACyberRemains::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		if (ActorIsSameType(BoxHit.GetActor())) return;

		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), m_Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(BoxHit);
		CreateFields(BoxHit.ImpactPoint);
	}

	//const FVector Start = m_leftBoxTraceStart->GetComponentLocation();
	//const FVector End = m_leftBoxTraceEnd->GetComponentLocation();

	//TArray<AActor*> ActorsToIgnore;
	//ActorsToIgnore.Add(this);
	//FHitResult BoxHit;
	//UKismetSystemLibrary::BoxTraceSingle(
	//	this,
	//	Start,
	//	End,
	//	FVector(5.f, 5.f, 5.f),
	//	m_leftBoxTraceStart->GetComponentRotation(),
	//	ETraceTypeQuery::TraceTypeQuery1,
	//	false,
	//	ActorsToIgnore,
	//	EDrawDebugTrace::ForDuration,
	//	BoxHit,
	//	true
	//);

}