// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "REnemy.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

/* generated */
#include "Projectile.generated.h"

UCLASS()
class PROJECTR_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	float m_Damage = 50.f;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* m_CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* m_ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* m_Tracer;

	class UParticleSystemComponent* m_TracerComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* m_ImpactParticles;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* m_nImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* m_ImpactSound;
};
