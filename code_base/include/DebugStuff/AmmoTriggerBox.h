// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTriggerBox.h"
#include "RChar.h"
#include "AmmoTriggerBox.generated.h"

/**
 *
 */
UCLASS()
class PROJECTR_API AAmmoTriggerBox : public ARTriggerBox
{
	GENERATED_BODY()

	AAmmoTriggerBox();

	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;
};
