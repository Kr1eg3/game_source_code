// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

/* generated */
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Attribute constructor
	/// </summary>
	UAttributeComponent();

	/// <summary>
	/// Tick function
	/// </summary>
	/// <param name="DeltaTime"></param>
	/// <param name="TickType"></param>
	/// <param name="ThisTickFunction"></param>
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Subtracts the damage value from the health value with clamp
	/// </summary>
	/// <param name="damage">The damage that will be received</param>
	void ReceiveDamage(const float damage);

	/// <summary>
	/// Returns the percentage ratio of current health to max health
	/// </summary>
	/// <returns></returns>
	float GetHealthPercent();

	/// <summary>
	/// Returns m_health > 0.f
	/// </summary>
	/// <returns></returns>
	bool IsAlive();

	/// <summary>
	/// Returns current health value
	/// </summary>
	/// <returns></returns>
	float GetHealthValue();

protected:
	virtual void BeginPlay() override;

// TODO: May be use uint for health and maxHealth values
private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float maxHealth;
};
