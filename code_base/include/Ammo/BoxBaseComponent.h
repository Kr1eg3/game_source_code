// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"

/* generated */
#include "BoxBaseComponent.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FBoxBaseParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxParameters")
	FVector boxExtent;

	FBoxBaseParameters()
	{
		boxExtent = { 10.f, 5.f, 5.f };
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UBoxBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBoxBaseComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UBoxComponent* getCollisionBoxPtr() const { return collisionBox; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxParameters")
	FBoxBaseParameters boxParameters;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* collisionBox;

};
