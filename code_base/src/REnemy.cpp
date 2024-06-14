// Fill out your copyright notice in the Description page of Project Settings.


#include "REnemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"


AREnemy::AREnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw	= false;

	m_pawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	m_pawnSensing->SightRadius = 4000.f;
	m_pawnSensing->SetPeripheralVisionAngle(45.f);

	m_goreSystem = CreateDefaultSubobject<UGoreComponent>(TEXT("GoreSystem"));

	m_CombatTarget = nullptr;

	bIsEnemyDisabled = false;
}

void AREnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));
	if (m_pawnSensing) m_pawnSensing->OnSeePawn.AddDynamic(this, &AREnemy::PawnSeen);

	InitializeEnemy();
}

void AREnemy::Die_Implementation()
{
	Super::Die_Implementation();

	m_EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	DisableCapsule();
	DisableMeshCollision();
	//SetLifeSpan(m_DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);

	// Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	SetLifeSpan(10.0f);
}

int32 AREnemy::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(deathMontage, deathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		m_DeathPose = Pose;
	}

	return Selection;
}

void AREnemy::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, m_AttackMontage);
	}
}

void AREnemy::PawnSeen(APawn* seenPawn)
{
	const bool bShouldChaseTarget =
		m_EnemyState != EEnemyState::EES_Dead &&
		m_EnemyState != EEnemyState::EES_Chasing &&
		m_EnemyState < EEnemyState::EES_Attacking &&
		seenPawn->ActorHasTag(FName("V9"));

	if (bShouldChaseTarget)
	{
		m_CombatTarget = seenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

void AREnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead() || bIsEnemyDisabled) return;
	if (m_EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}

	//DebugEnemyState();

}

void AREnemy::GetHit_Implementation(const FVector& impactPoint)
{
	Super::GetHit_Implementation(impactPoint);
	ClearPatrolTimer();
	ClearAttackTimer();

	StopAttackMontage();
	if (IsInsideAttackRadius())
	{
		if (!IsDead()) StartAttackTimer();
	}

}

float AREnemy::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	HandleDamage(damageAmount);
	if (attributes && !attributes->IsAlive())
	{
		Die();
		return 0;
	}
	m_CombatTarget = eventInstigator->GetPawn();
	ChaseTarget();

	return damageAmount;
}

bool AREnemy::ActorIsSameType(AActor* OtherActor)
{
	bool lval = ActorHasTag(TEXT("Enemy"));
	bool rval = OtherActor->ActorHasTag(TEXT("Enemy"));
	return lval && rval;
}

void AREnemy::InitializeEnemy()
{
	m_EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(m_PatrolTarget);
}

void AREnemy::CheckPatrolTarget()
{
	if ( InTargetRange(m_PatrolTarget, m_PatrolRadius) )
	{
		m_PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(m_PatrolWaitMin, m_PatrolWaitMax);
		GetWorldTimerManager().SetTimer(m_PatrolTimer, this, &AREnemy::PatrolTimerFinished, WaitTime);
	}
}

void AREnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(m_PatrolTimer);
}

void AREnemy::CheckCombatTarget()
{
	if ( attributes && !attributes->IsAlive() ) return;

	if ( IsOutsideCombatRadius() )
	{
		// Outside radius --> lose interest
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		// Outside attack range --> chase target
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		// Inside attack range --> attack
		StartAttackTimer();
		// TODO: Attack montage!
	}
}

void AREnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(m_AttackTimer);
}

void AREnemy::ChaseTarget()
{
	if (bIsEnemyDisabled) return;
	m_EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = m_ChasingSpeed;
	MoveToTarget(m_CombatTarget);
}

void AREnemy::ForceDie()
{
	Die_Implementation();
}

void AREnemy::PatrolTimerFinished()
{
	MoveToTarget(m_PatrolTarget);
}

void AREnemy::MoveToTarget(AActor* Target)
{
	if (m_EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(m_AcceptanceRadius);
	m_EnemyController->MoveTo(MoveRequest);
}

void AREnemy::LoseInterest()
{
	m_CombatTarget = nullptr;
}

void AREnemy::StartPatrolling()
{
	m_EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = m_PatrollingSpeed;
	MoveToTarget(m_PatrolTarget);
}

void AREnemy::StartAttackTimer()
{
	m_EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(m_AttackMin, m_AttackMax);
	GetWorldTimerManager().SetTimer(m_AttackTimer, this, &AREnemy::Attack, AttackTime);
}

AActor* AREnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : m_PatrolTargets)
	{
		if (Target != m_PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

bool AREnemy::IsAttacking()
{
	return m_EnemyState == EEnemyState::EES_Attacking;
}

bool AREnemy::IsChasing()
{
	return m_EnemyState == EEnemyState::EES_Chasing;
}

bool AREnemy::IsEngaged()
{
	return m_EnemyState == EEnemyState::EES_Engaged;
}

bool AREnemy::IsDead()
{
	return m_EnemyState == EEnemyState::EES_Dead;
}

bool AREnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

bool AREnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(m_CombatTarget, m_CombatRadius);
}

bool AREnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(m_CombatTarget, m_AttackRadius);
}

bool AREnemy::IsInsideAttackRadius()
{

	return InTargetRange(m_CombatTarget, m_AttackRadius);
}

void AREnemy::DebugEnemyState()
{
	switch (m_EnemyState)
	{
	case EEnemyState::EES_Dead:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy dead")));
		break;
	case EEnemyState::EES_Patrolling:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy patrolling")));
		break;
	case EEnemyState::EES_Chasing:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy chasing")));
		break;
	case EEnemyState::EES_Attacking:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy attacking")));
		break;
	case EEnemyState::EES_Engaged:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy engaged")));
		break;
	case EEnemyState::EES_NoState:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy no state")));
		break;
	default:
		break;
	}
}

void AREnemy::Attack()
{
	m_EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

int32 AREnemy::PlayAttackMontage()
{
	return PlayRandomMontageSection(m_AttackMontage, m_AttackMontageSections);
}

bool AREnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AREnemy::AttackEnd()
{
	m_EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

FVector AREnemy::GetTranslationWarpTarget()
{
	if (m_CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = m_CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= m_WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

FVector AREnemy::GetRotationWarpTarget()
{
	if (m_CombatTarget == nullptr) return FVector();

	return m_CombatTarget->GetActorLocation();
}

