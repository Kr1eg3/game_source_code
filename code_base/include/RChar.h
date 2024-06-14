// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define DEBUG 1

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractionInterface.h"
#include "CombatComponent.h"
#include "CombatStates.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "CommonFunctions.h"
#include "GameModes/RGameModeBase.h"

/* generated */
#include "RChar.generated.h"

DECLARE_MULTICAST_DELEGATE(FTestDelegate);
//DECLARE_MULTICAST_DELEGATE(FPlayerDied);

UENUM(BlueprintType)
enum class EMentalState : uint8
{
	EMS_Normal			UMETA(DisplayName	=		"Normal State"		),
	EMS_Scared			UMETA(DisplayName	=		"Scared"			),
	EMS_Terrified		UMETA(DisplayName	=		"Terrified"			),
	EMS_MAX				UMETA(DisplayName	=		"Default Max"		)
};

//class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UMeshComponent;
class UBoxComponent;

UCLASS()
class PROJECTR_API ARChar : public ABaseCharacter
{
	GENERATED_BODY()

public:
	/// @brief Sets default values for this character's properties
	ARChar();

	/// @brief Called every frame
	virtual void Tick(float DeltaTime) override;

	/// @brief Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void GetHit_Implementation(const FVector& impactPoint) override;

	virtual float TakeDamage(float damageAmount, struct FDamageEvent const& damageEvent,
		class AController* eventInstigator, AActor* damageCauser) override;

	void PlayReloadMontage();

	bool IsWeaponEquiped();
	bool IsAiming();
	FORCEINLINE float Getm_AO_Yaw() const { return m_AO_Yaw; }
	FORCEINLINE float Getm_AO_Pitch() const { return m_AO_Pitch; }
	FORCEINLINE USphereComponent* GetAimSphere() const { return m_AimSphere; }
	FORCEINLINE UCameraComponent* GetViewCamera() const { return m_ViewCamera; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return m_TurningInPlace; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return m_Combat;  }
	FORCEINLINE ECombatState GetCombatState() const;
	ARWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;

	//FPlayerDied playerDiedDelegate;
	FTestDelegate testDelegate;

private:
	FVector _getDirection(EAxis::Type _axis) const;

	void OnInteract();
	void TurnInPlace(float DeltaTime);
	float m_AO_Yaw;
	float InterpAO_Yaw;
	float m_AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace m_TurningInPlace;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* reloadMontage;


protected:
	/// @brief Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// @brief Forward/Backward movement function
	void MoveForward(float value);
	/// @brief Left/Right movement function
	void MoveRight(float value);
	/// @brief Turn Left/Right
	void Turn(float value);
	/// @brief Look Up/Dawn function
	void LookUp(float value);
	/// @brief Set mental state of robot as Normal
	void SetMStateNormal();
	/// @brief Set mental state of robot as Scared
	void SetMStateScared();
	/// @brief Set mental state of robot as Terrified
	void SetMStateTerrified();
	/// @brief Debug function to check current mental status
	void CheckMentalState();
	/// @brief Sprint function. Change MaxWalkSpeed: MaxWalkSpeed *= SprintSpeedMultiplier
	void Sprint();
	/// @brief Sprint function. Change MaxWalkSpeed: MaxWalkSpeed /= SprintSpeedMultiplier
	void StopSprinting();
	/// @brief Interact function. Shoots ray from character camera location
	void Interact();
	/// @brief Logic for aim button is pressed
	void AimButtonPressed();
	/// @brief Logic for aim button is released
	void AimButtonReleased();
	/// @brief Logic for fire button is pressed
	void FireButtonPressed();
	/// @brief Logic for fire button is released
	void FireButtonReleased();
	/// @brief Logic for reload button is pressed
	void ReloadButtonPressed();
	/// @brief Logic for reload button is released
	void ReloadButtonReleased();
	/// @brief Logic for fire button is pressed
	void CrouchButtonPressed();
	/// @brief Logic for fire button is released
	void AimOffset(float DeltaTime);

	virtual void Die_Implementation() override;

	inline void SetMentalState(EMentalState state) { m_MState = state; };

private:
	UPROPERTY(VisibleAnywhere)
	float m_InteractionRayLenght;

	UPROPERTY(VisibleAnywhere, Category="Character Movement: Walking")
	float m_SprintSpeedMultiplier;

	EMentalState m_MState;

	/// TODO: Fix this abomination
	TObjectPtr<UCharacterMovementComponent> MovementPtr = Cast<UCharacterMovementComponent>(this->GetCharacterMovement());

	//UMeshComponent* m_MeshPtr = Cast<UMeshComponent>(this->GetMesh());

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> m_CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> m_ViewCamera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_InteractionZone;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpotLightComponent> m_SpotLight;

//	UPROPERTY(EditAnywhere)
//	TObjectPtr<UStaticMeshComponent> m_EyeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> m_Combat;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UPointLightComponent> m_PointLight;

	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<IInteractionInterface> m_IInterface;
	IInteractionInterface* m_IInterface = nullptr;

	UPROPERTY(VisibleAnywhere)
	//TObjectPtr<AActor> m_NearestActor;
	AActor* m_NearestActor = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> m_AimSphere;
};
