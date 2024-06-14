// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "RChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatStates.h"
#include "RWeapon.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_Character = Cast<ARChar>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (m_Character == nullptr)
	{
		m_Character = Cast<ARChar>(TryGetPawnOwner());
	}
	if (m_Character == nullptr) return;

	FVector Vel = m_Character->GetVelocity();
	Vel.Z = 0.f;
	m_speed = Vel.Size();
	m_bIsAccelerating = m_Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	m_bIsInAir = m_Character->GetCharacterMovement()->IsFalling();
	m_bIsWeaponEquipped = m_Character->IsWeaponEquiped();
	m_EquippedWeapon = m_Character->GetEquippedWeapon();
	m_bIsAiming = m_Character->IsAiming();
	m_bIsCrouched = m_Character->bIsCrouched;
	m_TurningInPlace = m_Character->GetTurningInPlace();


	FRotator AimRotation = m_Character->GetBaseAimRotation();
	FRotator MovmentRotation = UKismetMathLibrary::MakeRotFromX(m_Character->GetVelocity());
	m_YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovmentRotation, AimRotation).Yaw;

	m_AO_Yaw = m_Character->Getm_AO_Yaw();
	m_AO_Pitch = m_Character->Getm_AO_Pitch();

	if (m_bIsWeaponEquipped && m_EquippedWeapon && m_EquippedWeapon->GetWeaponMesh() && m_Character->GetMesh())
	{
		LeftHandTransform = m_EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket", ERelativeTransformSpace::RTS_World));
		FVector OutPosition;
		FRotator OutRotation;
		m_Character->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		FTransform RightHandTransform = m_Character->GetMesh()->GetSocketTransform(FName("hand_r", ERelativeTransformSpace::RTS_World));
		FVector rhandLocation = RightHandTransform.GetLocation();
		FVector hitLocation = m_Character->GetAimSphere()->GetCenterOfMass();
		m_RightHandRotation = UKismetMathLibrary::FindLookAtRotation(rhandLocation, rhandLocation + (rhandLocation - hitLocation));


		// Draw debug lines
		FTransform MuzzleTipTransform = m_EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);
		FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
		//DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f, FColor::Red);
		DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), m_Character->GetHitTarget(), FColor::Green);

		bUseFABRIK = m_Character->GetCombatState() != ECombatState::ECS_RELOADING;

	}

}
