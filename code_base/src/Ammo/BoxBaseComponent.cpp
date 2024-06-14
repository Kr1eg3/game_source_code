// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/BoxBaseComponent.h"


UBoxBaseComponent::UBoxBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->SetBoxExtent(boxParameters.boxExtent);
	collisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}


void UBoxBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBoxBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

