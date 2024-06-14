// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"

/* generated */
#include "BulletCombatPartComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UBulletCombatPartComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBulletCombatPartComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float getForce() const { return force; };
	FORCEINLINE float getDamage() const { return damage; };
	UParticleSystem* getTracerPtr() { return tracer; };
	USoundCue* getImpactSoundPtr() { return impactSound; };
	UParticleSystem* getImpactParticlesPtr() { return impactParticles; };
	UNiagaraSystem* getNiagaraImpactParticlesPtr() { return nImpactParticles; };
	UParticleSystemComponent* getTracerComponentPtr() { return tracerComponent; };

	void playImpactSoundAtLocation(const FVector location);
	void spawnNiagaraImpactParticlesAtLocation(const FVector location, const FRotator rotator);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	float damage;

	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	float force;

	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	class UParticleSystem* tracer;

	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	class UParticleSystemComponent* tracerComponent;

	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	class UParticleSystem* impactParticles;

	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	class UNiagaraSystem* nImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
	class USoundCue* impactSound;
};
