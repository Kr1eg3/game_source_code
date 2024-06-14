// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleLightSwitch.h"

// Sets default values
AExampleLightSwitch::AExampleLightSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = m_RootComponent;

	m_LightSwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSwitchMesh"));
	m_LightSwitchMesh->SetupAttachment(RootComponent);

	m_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	m_Light->SetupAttachment(RootComponent);

	m_InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	m_InteractionWidget->SetupAttachment(RootComponent);
	m_InteractionWidget->SetVisibility(false);

	m_Light->SetIntensity(0);

	m_InteractionType = EInteractionTypes::EIT_Simple;
}

// Called when the game starts or when spawned
void AExampleLightSwitch::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AExampleLightSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExampleLightSwitch::Interact()
{
#if 1
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Yooo! Interaction happen!")));
#endif
	if (m_bIsOn)
	{
		m_Light->SetIntensity(0);
		m_bIsOn = false;
	}
	else
	{
		m_Light->SetIntensity(100000);
		m_bIsOn = true;
	}
}

void AExampleLightSwitch::ShowInteractionWidget()
{
	m_InteractionWidget->SetVisibility(true);
}

void AExampleLightSwitch::HideInteractionWidget()
{
	m_InteractionWidget->SetVisibility(false);
}
