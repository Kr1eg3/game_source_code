// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RWeapon.h"
#include "RProjectileWeapon.generated.h"

/**
 *
 */
UCLASS()
class PROJECTR_API ARProjectileWeapon : public ARWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& hitTarget) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> m_ProjectileClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> bulletCasing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletClass;
};
