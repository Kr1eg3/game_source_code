// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo/BulletCombatPartComponent.h"
#include "Kismet/GameplayStatics.h"


UBulletCombatPartComponent::UBulletCombatPartComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	damage = 50.f;
	force = 1000.f;
}

void UBulletCombatPartComponent::playImpactSoundAtLocation(const FVector location)
{
	if (impactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, impactSound, location);
	}
}

void UBulletCombatPartComponent::spawnNiagaraImpactParticlesAtLocation(const FVector location, const FRotator rotator)
{
	if (nImpactParticles) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner(), nImpactParticles, location, rotator, FVector(1.0f), true, true);
	}
}

void UBulletCombatPartComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBulletCombatPartComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

