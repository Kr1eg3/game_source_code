// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "RSpawner.h"

#include "RTriggerBox.generated.h"

/**
 *
 */
UCLASS()
class PROJECTR_API ARTriggerBox : public ATriggerBox
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:

	ARTriggerBox();

	UFUNCTION()
	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	virtual void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);


private:
	UPROPERTY(EditInstanceOnly, Category = "Spawner Properties")
	ARSpawner* m_Spawner;

};
