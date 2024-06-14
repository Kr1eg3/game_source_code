// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GoreComponent.h"
#include "HitInterface.h"
#include "RChar.h"

/* generated */
#include "REnemy.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead				UMETA(DisplayName = "Dead"),
	EES_Patrolling			UMETA(DisplayName = "Patrolling"),
	EES_Chasing				UMETA(DisplayName = "Chasing"),
	EES_Attacking			UMETA(DisplayName = "Attacking"),
	EES_Engaged				UMETA(DisplayName = "Engaged"),
	EES_NoState				UMETA(DisplayName = "NoState")
};


UCLASS()
class PROJECTR_API AREnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AREnemy();

	/********* <AActor> ****************/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float damageAmount, struct FDamageEvent const& damageEvent, class AController* eventInstigator, AActor* damageCauser) override;
	/********* </AActor> ****************/

	/********* <IHitInterface> ****************/
	virtual void GetHit_Implementation(const FVector& impactPoint) override;
	/********* </IHitInterface> ****************/

	void ChaseTarget();
	void ForceDie();

	UGoreComponent* getGoreSystem() { return m_goreSystem; };

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* m_CombatTarget;

protected:
	/********* <AActor> ****************/
	virtual void BeginPlay() override;
	/********* </AActor> ****************/

	/********* <ABaseCharacter> ****************/
	virtual void Die_Implementation() override;
	virtual int32 PlayDeathMontage() override;
	virtual void StopAttackMontage() override;
	/********* <ABaseCharacter> ****************/

	UFUNCTION()
	void PawnSeen(APawn* seenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> m_DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState m_EnemyState = EEnemyState::EES_Patrolling;

	bool ActorIsSameType(AActor* OtherActor);

private:

	/***** AI Behavior ******/
	virtual void InitializeEnemy();
	virtual void CheckPatrolTarget();
	virtual void ClearPatrolTimer();
	virtual void CheckCombatTarget();
	virtual void ClearAttackTimer();
	virtual void PatrolTimerFinished();
	virtual void MoveToTarget(AActor* Target);
	virtual void LoseInterest();
	virtual void StartPatrolling();
	virtual void StartAttackTimer();
	virtual AActor* ChoosePatrolTarget();
	virtual bool IsAttacking();
	virtual bool IsChasing();
	virtual bool IsEngaged();
	virtual bool IsDead();
	virtual bool InTargetRange(AActor* Target, double Radius);
	virtual bool IsOutsideCombatRadius();
	virtual bool IsOutsideAttackRadius();
	virtual bool IsInsideAttackRadius();
	virtual void DebugEnemyState();

	void Attack();
	int32 PlayAttackMontage();
	bool CanAttack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	// Patrol fields
	FTimerHandle m_PatrolTimer;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* m_PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> m_PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float m_PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float m_PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double m_PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double m_AcceptanceRadius = 50.f;

	UPROPERTY()
	class AAIController* m_EnemyController;

	// Combat fieds
	UPROPERTY(EditAnywhere, Category = Combat)
	bool bIsEnemyDisabled;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* m_AttackMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> m_AttackMontageSections;

	FTimerHandle m_AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	double m_WarpTargetDistance = 75.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double m_CombatRadius = 2000.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double m_AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float m_DeathLifeSpan = 8.f;

	/*
	*  Components
	*/
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* m_pawnSensing;

	UPROPERTY(VisibleAnywhere)
	UGoreComponent* m_goreSystem;
};
