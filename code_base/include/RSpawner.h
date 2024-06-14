// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "CyberRemains.h"

#define TEMPORARY_SOLUTION 1

#if TEMPORARY_SOLUTION
#include "Kismet/GameplayStatics.h"
#include "RChar.h"
#endif

#include "RSpawner.generated.h"

UCLASS()
class PROJECTR_API ARSpawner : public AActor
{
	GENERATED_BODY()

public:
	ARSpawner();

	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies();

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	int m_UnitAmountToSpawn;

	UPROPERTY(EditInstanceOnly, Category = "Spawner Properties")
	TArray<AActor*> m_SpawnSections;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACyberRemains> m_CyberRemains;

	std::vector<ACyberRemains*> m_SpawnedEnemies;

	// Temporary solution, remove it later
	ACharacter* m_Player;

};
