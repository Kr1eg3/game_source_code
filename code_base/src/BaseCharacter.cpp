// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

bool ABaseCharacter::IsAlive()
{
	return attributes && attributes->IsAlive();
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (IsAlive())
	{
		//DirectionalHitReact(Hitter->GetActorLocation());
		PlayHitReactMontage(FName("HitBody"));
	}
	else Die();
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (attributes)
	{
		attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(deathMontage, deathMontageSections);
}

void ABaseCharacter::StopAttackMontage() {}

void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && hitReactMontage)
	{
		animInstance->Montage_Play(hitReactMontage);
		animInstance->Montage_JumpToSection(SectionName, hitReactMontage);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage, 2.f);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


