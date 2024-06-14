// Fill out your copyright notice in the Description page of Project Settings.


#include "RProjectileWeapon.h"
#include "Projectile.h"
#include "Ammo/Bullet.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Ammo/Casing.h"

void ARProjectileWeapon::Fire(const FVector& hitTarget)
{
	Super::Fire(hitTarget);

	//APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	//const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	//if (MuzzleFlashSocket)
	//{
	//	FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	//	FVector ToTarget = hitTarget - SocketTransform.GetLocation();
	//	FRotator TargetRotation = ToTarget.Rotation();
	//	if (m_ProjectileClass && InstigatorPawn)
	//	{
	//		FActorSpawnParameters SpawnParams;
	//		SpawnParams.Owner = GetOwner();
	//		SpawnParams.Instigator = InstigatorPawn;
	//		UWorld* World = GetWorld();
	//		if (World)
	//		{
	//			World->SpawnActor<AProjectile>(
	//				m_ProjectileClass,
	//				SocketTransform.GetLocation(),
	//				TargetRotation,
	//				SpawnParams
	//				);
	//		}
	//	}
	//}

	// Spawn Casing
	if (bulletCasing)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = GetWeaponMesh()->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetWeaponMesh());
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ACasing>(
					bulletCasing,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
				);
			}
		}
	}

	// Spawn Patricle
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = hitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
		if (bulletClass && InstigatorPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ABullet>(
					bulletClass,
					SocketTransform.GetLocation(),
					TargetRotation,
					SpawnParams
				);
			}
		}
	}
}
