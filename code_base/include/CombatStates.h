#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_UNOCCUPIED	UMETA(DisplayName = "Unoccupied"  ),
	ECS_RELOADING	UMETA(DisplayName = "Reloading"   ),


	ECS_MAX			UMETA(DisplayName = "Default MAX" )
};
