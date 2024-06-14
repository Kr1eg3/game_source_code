// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"
#include "ExampleLightSwitch.generated.h"

UCLASS()
class PROJECTR_API AExampleLightSwitch : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExampleLightSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	virtual void ShowInteractionWidget() override;
	virtual void HideInteractionWidget() override;

	inline EInteractionTypes GetInteractionType() { return m_InteractionType; };

private:
	UPROPERTY(VisibleAnywhere)
	EInteractionTypes m_InteractionType;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> m_RootComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> m_LightSwitchMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPointLightComponent> m_Light;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> m_InteractionWidget;

	bool m_bIsOn = false;
};
