// Fill out your copyright notice in the Description page of Project Settings.


#include "RSpawner.h"

ARSpawner::ARSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	m_UnitAmountToSpawn = 1;
}

void ARSpawner::BeginPlay()
{
	Super::BeginPlay();

}

void ARSpawner::SpawnEnemies()
{
	for (auto* section : m_SpawnSections)
	{
		FVector SpawnLocation = section->GetActorLocation();
		FRotator SpawnRotation = section->GetActorRotation();
		m_Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		for (int i = 0; i < m_UnitAmountToSpawn; i++)
		{
			auto* enemy = GetWorld()->SpawnActor<ACyberRemains>(m_CyberRemains, SpawnLocation, SpawnRotation);
			if (m_Player && enemy)
			{
				enemy->m_CombatTarget = dynamic_cast<AActor*>(m_Player);
				enemy->ChaseTarget();
			}
		}


		//m_SpawnedEnemies.push_back(enemy);
	}
}

void ARSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

