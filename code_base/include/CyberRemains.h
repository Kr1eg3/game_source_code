// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "REnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CyberRemains.generated.h"

UCLASS()
class PROJECTR_API ACyberRemains : public AREnemy
{
	GENERATED_BODY()

public:
	ACyberRemains();

	UFUNCTION(BlueprintCallable)
	void SetLeftHitBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void SetRightHitBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void LeftHandStartAttacking() { m_bLeftHandAttack = true; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void LeftHandEndAttacking() { m_bLeftHandAttack = false; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void RightHandStartAttacking() { m_bRightHandAttack = true; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void RightHandEndAttacking() { m_bRightHandAttack = false; };

	FORCEINLINE UBoxComponent* GetLeftHitBox() const { return m_leftHitBox; };

	FORCEINLINE UBoxComponent* GetRightHitBox() const { return m_rightHitBox; };

	TArray<AActor*> m_IgnoreActors;

protected:
	virtual void BeginPlay() override;

	void BoxTrace(FHitResult& BoxHit);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Combat system
	UPROPERTY(EditAnywhere, Category = "Attack Damage")
	float m_Damage = 50.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_leftHitBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_leftBoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_leftBoxTraceEnd;

	UPROPERTY(VisibleAnywhere)
	bool m_bLeftHandAttack;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> m_rightHitBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_rightBoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_rightBoxTraceEnd;

	UPROPERTY(VisibleAnywhere)
	bool m_bRightHandAttack;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FVector m_BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool m_bShowBoxDebug = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack HitBoxes")
	//UBoxComponent* m_rightHand;

	UPROPERTY(EditAnywhere, Category = "Eyes Effect")
	UNiagaraSystem* nEyesEffect;

	UPROPERTY(EditAnywhere)
	FVector leftEyeLocation;

};
