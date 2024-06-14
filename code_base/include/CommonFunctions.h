// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPickupTypes : uint8
{
	EPT_Weapon     UMETA(DisplayName	=	 "Weapon"		),
	EPT_MAX		   UMETA(DisplayName	=	 "Default Max"	)
};

UENUM(BlueprintType)
enum class EInteractionTypes : uint8
{
	EIT_Weapon     UMETA(DisplayName	=	"Weapon"		),
	EIT_Simple     UMETA(DisplayName	=	"Simple"		),
	EIT_MAX		   UMETA(DisplayName	=	"Default Max"	)
};

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left		    UMETA(DisplayName = "Turning Left"),
	ETIP_Right			UMETA(DisplayName = "Turning Right"),
	ETIP_NotTurning		UMETA(DisplayName = "Not Turning"),
	ETIP_MAX			UMETA(DisplayName = "Default Max")
};

/**
 * 
 */
class PROJECTR_API CommonFunctions
{
public:
	CommonFunctions();
	~CommonFunctions();
};
