// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

/* generated */
#include "Bullet.generated.h"

UCLASS()
class PROJECTR_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:
	UPROPERTY(VisibleAnywhere)
	class UBoxBaseComponent* bulletShell;

	UPROPERTY(VisibleAnywhere)
	class UBulletCombatPartComponent* bulletCombatPart;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* bulletMovementComponent;

};
