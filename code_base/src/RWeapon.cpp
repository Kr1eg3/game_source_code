// Fill out your copyright notice in the Description page of Project Settings.


#include "RWeapon.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"

class UCombatComponent;

ARWeapon::ARWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	m_WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(m_WeaponMesh);
	m_WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	m_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	m_AreaSphere->SetupAttachment(RootComponent);
	m_AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	m_AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	m_InteractionWidget->SetupAttachment(RootComponent);
	m_InteractionWidget->SetVisibility(false);

	m_InteractionType = EInteractionTypes::EIT_Weapon;
	ammoInMag = 10;
	magCapacity = 10;
}

void ARWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Enable collision
	m_AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ARWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARWeapon::Interact() { }

void ARWeapon::ShowInteractionWidget()
{
	m_InteractionWidget->SetVisibility(true);
}

void ARWeapon::HideInteractionWidget()
{
	m_InteractionWidget->SetVisibility(false);
}

void ARWeapon::Fire(const FVector& hitTarget)
{
	if (m_FireAnimation)
	{
		m_WeaponMesh->PlayAnimation(m_FireAnimation, false);
	}

	// Ammo calculation
	SpendRound();

}

void ARWeapon::SpendRound()
{
	ammoInMag = FMath::Clamp(ammoInMag - 1, 0, magCapacity);
}

void ARWeapon::DisableCollision()
{
	m_WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

bool ARWeapon::IsEmpty()
{
	return !ammoInMag;
}

