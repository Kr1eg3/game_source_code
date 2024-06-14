// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "WeaponTypes.h"
#include "CombatStates.h"
#include "CombatComponent.generated.h"

class ARChar;
class ARWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class ARChar;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void StartFireTimer();
	void FireTimerFinished();
	UFUNCTION(BlueprintCallable)
	void FinishReload();
	void ResetFullAmmo();

	FORCEINLINE void SetCurrentAmmoCarried(uint16 ammoAmount) { currentAmmoCarried = ammoAmount; };

	FORCEINLINE uint16 GetMaxAmmoCarried() { return maxAmmoCarried; };

protected:
	virtual void BeginPlay() override;
	void EquipWeapon(ARWeapon* weaponToEquip);
	void FireButtonPressed(bool bIsPressed);
	void ReloadButtonPressed();
	void TraceUnderCrosshairs(FHitResult& traceHitResult);
	void InterpFOV(float DeltaTime);
	bool CanFire();
	void Fire();
	void InitCarriedAmmo();
	void HandleReload();
	int32 AmountToReload();

private:
	class ARChar* m_Character;

	UPROPERTY()
	TObjectPtr<ARWeapon> m_EquippedWeapon;

	bool m_bIsAiming;

	bool m_bIsFireButtonPressed;

	FVector m_HitTarget;

	float m_DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_ZoomInterpSpeed = 20.f;

	float m_CurrentFOV;

	FTimerHandle m_FireTimer;

	bool m_bCanFire = true;

	UPROPERTY(EditAnywhere, Category = "Ammo Properties")
	uint16 currentAmmoCarried;

	UPROPERTY(EditAnywhere, Category = "Ammo Properties")
	uint16 maxAmmoCarried;

	TMap<EWeaponType, int32> carriedAmmoMap;

	ECombatState combatState = ECombatState::ECS_UNOCCUPIED;
};
