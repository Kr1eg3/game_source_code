// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HitInterface.h"
#include "Kismet/GameplayStatics.h"

/* generated */
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Death1			    UMETA(DisplayName = "Death1"),
	EDP_Death2			    UMETA(DisplayName = "Death2"),
	EDP_MAX					UMETA(DisplayName = "DefaultMAX")
};

class UAnimMontage;

UCLASS()
class PROJECTR_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/********************************************************************************
		   ______                __          __     _____           __  _
		  / ____/___  ____ ___  / /_  ____ _/ /_   / ___/___  _____/ /_(_)___  ____
		 / /   / __ \/ __ `__ \/ __ \/ __ `/ __/   \__ \/ _ \/ ___/ __/ / __ \/ __ \
		/ /___/ /_/ / / / / / / /_/ / /_/ / /_    ___/ /  __/ /__/ /_/ / /_/ / / / /
		\____/\____/_/ /_/ /_/_.___/\__,_/\__/   /____/\___/\___/\__/_/\____/_/ /_/
	*********************************************************************************/
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	bool IsAlive();
	void DisableCapsule();
	void DisableMeshCollision();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void HandleDamage(const float DamageAmount);

	// Play montages functions
	virtual int32 PlayDeathMontage();
	virtual void  StopAttackMontage();
	void  PlayHitReactMontage(const FName& SectionName);
	void  PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	/********************************************************************************
		   ______                                             __
		  / ____/___  ____ ___  ____  ____  ____  ___  ____  / /______
		 / /   / __ \/ __ `__ \/ __ \/ __ \/ __ \/ _ \/ __ \/ __/ ___/
		/ /___/ /_/ / / / / / / /_/ / /_/ / / / /  __/ / / / /_(__  )
		\____/\____/_/ /_/ /_/ .___/\____/_/ /_/\___/_/ /_/\__/____/
						/_/
	*********************************************************************************/
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attributes;

	/********************************************************************************
			__  ___            __
		   /  |/  /___  ____  / /_____ _____ ____  _____
		  / /|_/ / __ \/ __ \/ __/ __ `/ __ `/ _ \/ ___/
		 / /  / / /_/ / / / / /_/ /_/ / /_/ /  __(__  )
		/_/  /_/\____/_/ /_/\__/\__,_/\__, /\___/____/
									 /____/
	*********************************************************************************/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* hitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> deathMontageSections;
};
