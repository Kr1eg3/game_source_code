// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class PROJECTR_API ACasing : public AActor
{
	GENERATED_BODY()

public:
	ACasing();

protected:
	virtual void BeginPlay() override;

	void DestroyCasing();

private:
	FTimerHandle slowMoTimer;

	UPROPERTY(EditAnywhere)
	float destroyTime;

	UPROPERTY(EditAnywhere)
	float ejectionImpulse;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* casingMesh;

};
