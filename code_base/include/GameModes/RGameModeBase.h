// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
//#include "RSpawner.h"
#include "RGameModeBase.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogProjectRGameMode, Log, All);



/**
 *
 */
UCLASS()
class PROJECTR_API ARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	ARGameModeBase();
	void HandlePlayerDeath();
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable, Category = Game)
	//virtual void ResetLevel() override;




	UPROPERTY(EditAnywhere, Category = "Game Properties")
	uint32 maxNumOfEnemies;

	UPROPERTY(EditAnywhere, Category = "Game Properties")
	uint32 spawnEnemiesDelay;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle spawnTimerHandle;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	//UPROPERTY(EditInstanceOnly, Category = "Spawner Properties")
	//ARSpawner* m_Spawner;






};
