// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "CommonFunctions.h"
#include "Components/WidgetComponent.h"
#include "WeaponTypes.h"
#include "RWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial  UMETA(DisplayName	= "Initial State"	),
	EWS_Equipped UMETA(DisplayName	= "Equipped"		),
	EWS_Dropped  UMETA(DisplayName	= "Dropped"			),
	EWS_MAX		 UMETA(DisplayName	= "Default Max"		)
};

UCLASS()
class PROJECTR_API ARWeapon : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ARWeapon();
	friend class UCombatComponent;
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	virtual void ShowInteractionWidget() override;
	virtual void HideInteractionWidget() override;
	virtual void Fire(const FVector& hitTarget);
	virtual void SpendRound();
	void DisableCollision();
	bool IsEmpty();

	FORCEINLINE void SetWeaponState(EWeaponState state) { m_WeaponState = state; };
	FORCEINLINE void SetAmmoInMag(const int32 ammoAmount) { ammoInMag = ammoAmount; };

	FORCEINLINE EInteractionTypes GetInteractionType() { return m_InteractionType; };
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return m_WeaponMesh; };
	FORCEINLINE int32 GetAmmoInMag() const { return ammoInMag; };
	FORCEINLINE int32 GetMagCapacity() const { return magCapacity; };
	FORCEINLINE float GetZoomedFOV() const { return m_ZoomedFOV; };
	FORCEINLINE float GetZoomInterpSpeed() const { return m_ZoomInterpSpeed; };
	FORCEINLINE EWeaponType GetWeaponType() const { return weaponType; };

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool m_bIsAutomatic = true;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	EWeaponState m_WeaponState;

	UPROPERTY(VisibleAnywhere)
	EInteractionTypes m_InteractionType;

	UPROPERTY(EditAnywhere)
	EWeaponType weaponType;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> m_WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* m_AreaSphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> m_InteractionWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* m_FireAnimation;

	UPROPERTY(EditAnywhere)
	float m_ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float m_ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Ammo Properties")
	uint16 ammoInMag;

	UPROPERTY(EditAnywhere, Category = "Ammo Properties")
	uint16 magCapacity;



};
