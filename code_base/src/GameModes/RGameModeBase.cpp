// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/RGameModeBase.h"
#include "RSpawner.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY(LogProjectRGameMode);

//void ARGameModeBase::ResetLevel()
//{
//
//	Super::ResetLevel();
//}

TArray<AActor*> myEnemySpawners;

ARGameModeBase::ARGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARGameModeBase::HandlePlayerDeath()
{
	auto levelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(levelName));
}

void ARGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWorld()->GetTimerManager().IsTimerActive(spawnTimerHandle))
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &ARGameModeBase::SpawnEnemy, spawnEnemiesDelay, false);
}

void ARGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSpawner::StaticClass(), myEnemySpawners);
}

void ARGameModeBase::SpawnEnemy()
{
	UE_LOG(LogProjectRGameMode, Warning, TEXT("Spawning Enemies"));
	Cast<ARSpawner>(myEnemySpawners[FMath::FRandRange(0, myEnemySpawners.Num())])->SpawnEnemies();
}
