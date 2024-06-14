// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/Casing.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	casingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(casingMesh);

	casingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	casingMesh->SetSimulatePhysics(true);
	casingMesh->SetEnableGravity(true);

	ejectionImpulse = FMath::RandRange(4.f, 10.f);
	destroyTime = 4.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	casingMesh->AddImpulse(GetActorForwardVector() * ejectionImpulse);
	GetWorld()->GetTimerManager().SetTimer(slowMoTimer, this, &ACasing::DestroyCasing, destroyTime);
}

void ACasing::DestroyCasing()
{
	Destroy();
}

