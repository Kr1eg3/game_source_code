// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	// Dont forget to remove ticks!!!
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(float damage)
{
	health = FMath::Clamp(health - damage, 0.f, maxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return health / maxHealth;
}

bool UAttributeComponent::IsAlive()
{
	return health > 0.f;
}

float UAttributeComponent::GetHealthValue()
{
	return health;
}

