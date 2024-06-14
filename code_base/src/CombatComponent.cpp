// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "RWeapon.h"
#include "RChar.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#define TRACE_LENGTH 80000.f

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_Character)
	{
		if (m_Character->GetViewCamera())
		{
			m_DefaultFOV = m_Character->GetViewCamera()->FieldOfView;
			m_CurrentFOV = m_DefaultFOV;
		}

		InitCarriedAmmo();
	}
}

void UCombatComponent::EquipWeapon(ARWeapon* weaponToEquip)
{
	if (m_Character == nullptr || weaponToEquip == nullptr) return;

	m_EquippedWeapon = weaponToEquip;
	m_EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* handSocket = m_Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (handSocket)
	{
		handSocket->AttachActor(m_EquippedWeapon, m_Character->GetMesh());
	}
	m_EquippedWeapon->SetOwner(m_Character);
	m_EquippedWeapon->SetInstigator(m_Character);
	m_EquippedWeapon->HideInteractionWidget();

	// Disable collision
	m_EquippedWeapon->DisableCollision();
	m_EquippedWeapon->GetWeaponMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	m_Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	m_Character->bUseControllerRotationYaw = true;

	if (carriedAmmoMap.Contains(m_EquippedWeapon->GetWeaponType()))
	{
		maxAmmoCarried = carriedAmmoMap[m_EquippedWeapon->GetWeaponType()];
	}

	//maxAmmoCarried = 90;
	currentAmmoCarried = maxAmmoCarried;
}

void UCombatComponent::FireButtonPressed(bool bIsPressed)
{
	m_bIsFireButtonPressed = bIsPressed;

	if (m_EquippedWeapon == nullptr) return;
	if (m_Character && m_bIsFireButtonPressed)
	{
		Fire();
		//m_EquippedWeapon->Fire(m_HitTarget);
		//StartFireTimer();
	}
}

void UCombatComponent::ReloadButtonPressed()
{
	if (!m_Character && !m_EquippedWeapon) return;

	if (currentAmmoCarried > 0 && combatState != ECombatState::ECS_RELOADING)
	{
		combatState = ECombatState::ECS_RELOADING;
		HandleReload();
	}
}

void UCombatComponent::HandleReload()
{
	int32 amountToReload = AmountToReload();
	m_EquippedWeapon->SetAmmoInMag(amountToReload);
	currentAmmoCarried -= amountToReload;
	m_Character->PlayReloadMontage();
}

int32 UCombatComponent::AmountToReload()
{
	if (!m_EquippedWeapon) return 0;

	//int32 roomInMag = m_EquippedWeapon->GetMagCapacity() - m_EquippedWeapon->GetAmmoInMag();
	//if (!roomInMag) return 0;

	int32 amount = FMath::Clamp(m_EquippedWeapon->GetMagCapacity(), 0, currentAmmoCarried);

	return amount;
}

void UCombatComponent::ResetFullAmmo()
{
	if (!m_Character && !m_EquippedWeapon) return;
	SetCurrentAmmoCarried(maxAmmoCarried);
}

void UCombatComponent::FinishReload()
{
	if (!m_Character) return;
	combatState = ECombatState::ECS_UNOCCUPIED;

	if (m_bIsFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& traceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition, CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
		);
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		GetWorld()->LineTraceSingleByChannel(
			traceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
			);

		if (!traceHitResult.bBlockingHit)
		{
			traceHitResult.ImpactPoint = End;
			m_HitTarget = End;
		}
		else
		{
			m_HitTarget = traceHitResult.ImpactPoint;
			//DrawDebugSphere(
				//GetWorld(),
				//traceHitResult.ImpactPoint,
				//12.f,
				//12,
				//FColor::Red
				//);
		}
	}
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	if (m_EquippedWeapon == nullptr) return;

	if (m_bIsAiming)
	{
		m_CurrentFOV = FMath::FInterpTo(m_CurrentFOV, m_EquippedWeapon->GetZoomedFOV(), DeltaTime, m_EquippedWeapon->GetZoomInterpSpeed());
	}
	else
	{
		m_CurrentFOV = FMath::FInterpTo(m_CurrentFOV, m_DefaultFOV, DeltaTime, m_ZoomInterpSpeed);
	}

	if (m_Character && m_Character->GetViewCamera())
	{
		m_Character->GetViewCamera()->SetFieldOfView(m_CurrentFOV);
	}
}

bool UCombatComponent::CanFire()
{
	if (!m_EquippedWeapon) return false;
	return !m_EquippedWeapon->IsEmpty() && m_bCanFire && combatState == ECombatState::ECS_UNOCCUPIED;
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		m_bCanFire = false;
		m_EquippedWeapon->Fire(m_HitTarget);
		StartFireTimer();

		// Recoil part
		float Recoil = .2f;
		float FinalRecoil = Recoil * FMath::FRandRange(-1.0f, -1.15f);
		m_Character->AddControllerPitchInput(FinalRecoil);
	}
}

void UCombatComponent::InitCarriedAmmo()
{
	carriedAmmoMap.Emplace(EWeaponType::EWT_ASSAULTRIFLE, 90);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_Character)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		m_HitTarget = HitResult.ImpactPoint;
		InterpFOV(DeltaTime);
	}
}

void UCombatComponent::StartFireTimer()
{
	if (m_EquippedWeapon == nullptr && m_Character == nullptr) return;

	m_Character->GetWorldTimerManager().SetTimer(
		m_FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		m_EquippedWeapon->m_FireDelay
	);
}

void UCombatComponent::FireTimerFinished()
{
	if (m_EquippedWeapon == nullptr) return;

	m_bCanFire = true;
	if (m_bIsFireButtonPressed && m_EquippedWeapon->m_bIsAutomatic)
	{
		Fire();
	}
}

