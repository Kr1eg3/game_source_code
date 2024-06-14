// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

/* generated */
#include "GoreComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUEGoreSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoneBroken, FName, Bone, class USkeletalMeshComponent*, DetachedLimb);


USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemDetachedReference
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TMap<FName, USkeletalMeshComponent*> References;

	FGoreSystemDetachedReference()
	{
		References = TMap<FName, USkeletalMeshComponent*>();
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemDecoArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TArray<UStreamableRenderAsset*> Decos;

	FGoreSystemDecoArray()
	{
		Decos = TArray<UStreamableRenderAsset*>();
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemHealthValues
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TArray<FName> BoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TArray<float> Health;

	FGoreSystemHealthValues()
	{
		BoneName = TArray<FName>();
		Health = TArray < float>();
	}

	FGoreSystemHealthValues(const TArray < FName> InName, const TArray<float> InHealth)
	{
		BoneName = InName;
		Health = InHealth;
	}

	bool Contains(const FName InName)
	{
		return BoneName.Contains(InName);
	}

	bool GetHealth(const FName InName, float& OutHealth)
	{
		if (!BoneName.Contains(InName))
			return false;

		OutHealth = Health[BoneName.IndexOfByKey(InName)];
		return true;
	}

	bool SetHealth(const FName InName, float InHealth)
	{
		if (!BoneName.Contains(InName))
			return false;

		Health[BoneName.IndexOfByKey(InName)] = InHealth;
		return true;
	}

	bool SetHealth(const TTuple<FName, float> InValue)
	{
		if (!BoneName.Contains(InValue.Key))
			return false;

		Health[BoneName.IndexOfByKey(InValue.Key)] = InValue.Value;
		return true;
	}

	bool Add(const FName InName, float InHealth)
	{
		if (BoneName.Contains(InName))
			return false;

		BoneName.Add(InName);
		Health.Add(InHealth);
		return true;
	}

	bool Add(const TTuple<FName, float> InValue)
	{
		if (BoneName.Contains(InValue.Key))
			return false;

		BoneName.Add(InValue.Key);
		Health.Add(InValue.Value);
		return true;
	}
};

FORCEINLINE bool operator==(const FGoreSystemHealthValues& Param1, const FGoreSystemHealthValues& Param2)
{
	return Param1.BoneName == Param2.BoneName;
}

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemHit
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FName BoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FVector Normal;

	FGoreSystemHit()
	{
		BoneName = FName();
		Location = FVector(0.0f);
		Normal = FVector(0.0f);
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemSounds
{
	GENERATED_USTRUCT_BODY()

	/** Sound on hit*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class USoundCue* Hit;
	/** Sound on destroy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class USoundCue* Break;

	FGoreSystemSounds()
	{
		Hit = nullptr;
		Break = nullptr;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemEffects
{
	GENERATED_USTRUCT_BODY()

	/** Niagara effect to spawn on Hit*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class UNiagaraSystem* Hit;
	/** Legacy cascade effect to spawn on Hit*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class UParticleSystem* HitLegacy;
	/** Niagara effect to spawn and attach on Destroy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class UNiagaraSystem* Spill;
	/** Niagara effect to spawn particles effect on Destroy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class UNiagaraSystem* GoreParticles;
	/** Legacy cascade effect to spawn on Spill*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class UParticleSystem* SpillLegacy;
	/** Decal to spawn (both)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TSubclassOf<class ADecalActor> Decal;
	/** Time before destroying the decals*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	float DecalLifespan;

	FGoreSystemEffects()
	{
		Hit = nullptr;
		HitLegacy = nullptr;
		Spill = nullptr;
		GoreParticles = nullptr;
		SpillLegacy = nullptr;
		Decal = NULL;
		DecalLifespan = 25.0f;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemStruct
{
	GENERATED_USTRUCT_BODY()

	/** Mesh to use once the detached limb is spawned*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	class USkeletalMesh* Mesh;
	/** Override materials*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TArray<UMaterialInterface*> MaterialsOverride;
	/** Deco mesh list to spawn when detach happen. You can spawn multiple decos for each entry. [USING CUSTOM SOCKETS INSTEAD OF THE SKELETAL'S BONES IS ADVICED]*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TMap<FName, FGoreSystemDecoArray> DecoMesh;
	/** Should override VFXs for this bone?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	uint8 OverrideEffects : 1;
	/** New VFXs to use*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem", meta = (EditCondition = "OverrideEffects"))
	FGoreSystemEffects Effects;
	/** Should override VFXs for this bone?*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	uint8 OverrideSounds : 1;
	/** New SFXs to use*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem", meta = (EditCondition = "OverrideSounds"))
	FGoreSystemSounds Sounds;

	/** [READ ONLY] Detached mesh, if spawned*/
	UPROPERTY(BlueprintReadOnly, Category = "GoreSystem")
	class USkeletalMeshComponent* DetachedMesh;

	FGoreSystemStruct()
	{
		Mesh = nullptr;
		MaterialsOverride = TArray<UMaterialInterface*>();
		OverrideEffects = false;
		Effects = FGoreSystemEffects();
		OverrideSounds = false;
		Sounds = FGoreSystemSounds();

		DetachedMesh = nullptr;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGoreSystemSettings
{
	GENERATED_USTRUCT_BODY()

	/** VFXs to spawn for all the bones (can be overridden for each "BodyMap" entry)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FGoreSystemEffects GlobalEffects;
	/** SFXs to spawn for all the bones (can be overridden for each "BodyMap" entry)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FGoreSystemSounds GlobalSounds;

	///** Animation instance you want to use for detached limbs (must derive from UUEGoreSystemAnimInstance and make sure to set it up accordingly to the example and documentation)*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	//class TSubclassOf<UUEGoreSystemAnimInstance> GlobalAnimInstance;

	/** Initial health to use for all the bones (can be overridden using "HealthMap")*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	float GlobalLimbsHealth;

	/** Settings for each bone you want to detach with this system*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TMap<FName, FGoreSystemStruct> BodyMap;
	/** List of bones that can be detached even if the character is still alive*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TArray<FName> IgnoreIsDead;
	/** Override "GlobalLimbsHealth" for each bone*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TMap<FName, float> HealthMap;
	/** Apply a custom damage multiplier for each entry on this list*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TMap<FName, float> DamageMultiplierMap;
	/** Collision profile name to use for detached limbs once physics is enabled ("Ragdoll" is default)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FName LimbsCollisionProfile;
	/** Collision profile name to use for decos spawned after detachment ("NoCollision" is default)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	FName DecoCollisionProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoreSystem")
	TArray<TEnumAsByte<ECollisionChannel>> DecalCollisionChannels;

	FGoreSystemSettings()
	{
		DamageMultiplierMap = TMap<FName, float>();
		GlobalEffects = FGoreSystemEffects();
		GlobalSounds = FGoreSystemSounds();
		//GlobalAnimInstance = UUEGoreSystemAnimInstance::StaticClass();
		GlobalLimbsHealth = 1.0f;
		BodyMap = TMap<FName, FGoreSystemStruct>();
		IgnoreIsDead = TArray<FName>();
		DecoCollisionProfile = FName(TEXT("NoCollision"));
		LimbsCollisionProfile = FName(TEXT("Ragdoll"));
		DecalCollisionChannels.Add(ECollisionChannel::ECC_WorldStatic);
		DecalCollisionChannels.Add(ECollisionChannel::ECC_WorldDynamic);
	}
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API UGoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGoreComponent();


	/** Should enable debuging?*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem")
	uint8 bEnableDebugLogging : 1;

	/** Physics asset to use, will override any other*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem")
	UPhysicsAsset* DefaultPhysicsAsset;

	/** Should enable automatic dismemberment? (beta)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem|Automation")
	uint8 bEnableAutoDismemberment : 1;

	/** Should enable stretching fix? (beta)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem|Automation", meta = (EditCondition = "bEnableAutoDismemberment"))
	uint8 bEnableStretchingFix : 1;

	/** Should enable debuging?*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem|Automation", meta = (EditCondition = "bEnableAutoDismemberment"))
	uint8 bControlRigProcedural : 1;

	/** Replace your character mesh with another "wounded" version for your detached limbs*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem|Automation", meta = (EditCondition = "bEnableAutoDismemberment"))
	USkeletalMesh* LimbsMesh;

	/** Main data structure for this GoreSystem Component*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GoreSystem")
	FGoreSystemSettings GoreSettings;

	UPROPERTY(BlueprintReadOnly, Category = "GoreSystem|Runtime")
	FGoreSystemHealthValues HealthValues;

	/** [SERVER ONLY] Get Health of a specific bone
	* >BoneName - Target bone
	* @Health - Return Health value
	* @return true if bone has been found, false if not;
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	bool GetBoneHealth(FName BoneName, float& Health) const {
		if (!GetOwner()->HasAuthority())
			return false;

		if (GoreSettings.BodyMap.Contains(BoneName))
		{
			if (HealthValues.BoneName.Contains(BoneName)) {
				Health = HealthValues.Health[HealthValues.BoneName.IndexOfByKey(BoneName)];
			}
			else {
				if (GoreSettings.HealthMap.Contains(BoneName)) {
					Health = GoreSettings.HealthMap.FindRef(BoneName);
				}
				else {
					Health = GoreSettings.GlobalLimbsHealth;
				}
			}
			return true;
		}
		return false;
	}

	/** Get Detached Mesh Component from BoneName
	* >BoneName - Target bone
	* @MeshComponent - Returns pointer to mesh component
	* @return true if the mesh component exists;
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	bool GetDetachedMeshFromBoneName(FName BoneName, USkeletalMeshComponent*& MeshComponent) const {

		if (!GoreSettings.BodyMap.Find(BoneName))
			return false;

		MeshComponent = GoreSettings.BodyMap.Find(BoneName)->DetachedMesh;
		return MeshComponent->IsValidLowLevelFast();
	}

	UPROPERTY()
	TArray<class UFXSystemComponent*> AttachedGoreFXs;

	UFUNCTION(BlueprintCallable, Category = "GoreSystem", BlueprintAuthorityOnly)
	void DestroyAllAttachedGoreFXs();
	UFUNCTION(NetMulticast, Reliable)
	void DestroyAllAttachedGoreFXs_Multi();

	UPROPERTY()
	TArray<FName> DestroyedBones;
	UPROPERTY()
	TArray<ADecalActor*> DecalsList;
	UPROPERTY()
	TArray<USceneComponent*> SpawnedComponents;
	UPROPERTY()
	USkeletalMeshComponent* VisualMeshComponent;

	UFUNCTION(BlueprintPure, Category = "GoreSystem")
	USkeletalMeshComponent* GetVisualMeshComponent() const {
		return VisualMeshComponent;
	}

	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void SetVisualMeshComponent(USkeletalMeshComponent* InComp) {
		VisualMeshComponent = InComp;
	}


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "GoreSystem|Runtime")
	class USkeletalMeshComponent* MeshRoot;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "GoreSystem")
	USkeletalMeshComponent* GetMeshRoot() const {
		return MeshRoot;
	}

	/** [Optional] This function will initialize the GoreSystem, it's automatically called OnBeginPlay but you can use it to manually setup the target SkeletalMeshComponent you want to use
	* >NewRootMesh - SkeletalMeshComponent to use by the GoreSystem
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void Init(USkeletalMeshComponent* NewRootMesh);

	UFUNCTION()
	void SpawnDecalAt(const TSubclassOf<ADecalActor> DecalActorClass, const FTransform DecalTransform, const float InLifespan);

	/** Hit multiple bones, spawning VFXs and SFXs for each socket in the list [THIS WILL NOT AFFECT LIMBS HEALTH]
	* >Bones - BoneName, Location and Normal to use for these Hit events. Normally these informations are gathered from a trace or hit event depending on your damage system!
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void HitBones(const TArray<FGoreSystemHit> Bones);
	UFUNCTION(Server, Reliable)
	void HitBones_Serv(const TArray<FGoreSystemHit>& Bones);
	UFUNCTION(NetMulticast, Reliable)
	void HitBones_Multi(const TArray<FGoreSystemHit>& Bones);
	UFUNCTION()
	void Int_HitBones(const TArray<FGoreSystemHit> Bones);

	/** Hit a single bone, spawning VFXs and SFXs [THIS WILL NOT AFFECT LIMBS HEALTH]
	* >Bone - BoneName, Location and Normal to use for this Hit event. Normally these informations are gathered from a trace or hit event depending on your damage system!
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void HitBone(const FGoreSystemHit BoneHit);

	/** Destroy multiple bones, spawning VFXs and SFXs for each socket in the list [THIS WILL NOT AFFECT LIMBS HEALTH]
	* >Bones - BoneName, Location and Normal to use for these Destroy event. Normally these informations are gathered from a trace or hit event depending on your damage system!
	* >InForce - Force to apply on detachment
	* @return true if detach happened, false if not;
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	bool DestroyBones(const TArray<FGoreSystemHit> Bones, const float Force = 3000.0f);
	UFUNCTION(Server, Reliable)
	void DestroyBones_Serv(const TArray<FGoreSystemHit>& Bones, const float Force);
	UFUNCTION(NetMulticast, Reliable)
	void DestroyBones_Multi(const TArray<FGoreSystemHit>& Bones, const float Force);
	UFUNCTION()
	void Int_DestroyBones(const TArray<FGoreSystemHit> Bones, const float Force);
	UFUNCTION()
	void Int_BoneHide(const FName Bone, USkeletalMeshComponent* MeshComp);

	/** Destroy a single bone, spawning VFXs and SFXs [THIS WILL NOT AFFECT LIMBS HEALTH]
	* >Bone - BoneName, Location and Normal to use for this Destroy event. Normally these informations are gathered from a trace or hit event depending on your damage system!
	* >InForce - Force to apply on detachment
	* @return true if detach happened, false if not;
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	bool DestroyBone(const FGoreSystemHit InBone, const float InForce = 3000.0f);

	UFUNCTION()
	class UPhysicsAsset* ClonePhysicsAsset(const UPhysicsAsset* PhysicsAssetToClone, const FName RemoveBefore);

	/** Damage multiple bones with multiple damage values, this function will only modify the health value stored in the system
	* No VFXs or SFXs involved
	* >BonesDamage - Double Array pairing BoneName and Damage, you should use this variable as a TMap
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void DamageBonesMultipleDamages(const FGoreSystemHealthValues BonesDamage);

	/** Damage multiple bones, this function will only modify the health value stored in the system
	* No VFXs or SFXs involved
	* >InBones - List of bones to damage
	* >InDamage - Damage to apply to each bone
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void DamageBonesSingleDamage(const TArray<FName> InBones, const float InDamage);

	UFUNCTION(Server, Reliable)
	void DamageBones_Serv(const FGoreSystemHealthValues BonesDamage);

	/** Damage a single bone, this function will only modify the health value stored in the system
	* No VFXs or SFXs involved
	* >InBoneName - Bone to damage
	* >InDamage - Input damage to use
	* @NewBoneHealth - Return the new health of this bone after damage has been appied
	* @DamageApplied - Return the total amount of damage appied
	* @return true if damage has been applied, false if not
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	bool DamageBone(const FName InBoneName, const float InDamage, float& NewBoneHealth, float& DamageApplied);

	/** Damage a single bone
	* VFXs and SFXs involved
	* >Damage - Input damage to use
	* >IsDead - The character is dead and the system can proceed on detaching any limb on the list?
	* >DetachmentForce - Force to apply on detachment
	* >BoneHit - BoneName, Location and Normal to use for this Destroy event. Normally these informations are gathered from a trace or hit event depending on your damage system!
	* @NewBoneHealth - Return the new health of this bone after damage has been appied
	* @DamageApplied - Return the total amount of damage appied
	* @return true if damage has been applied, false if not
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem", BlueprintAuthorityOnly)
	bool ApplyGoreDamage(const float Damage, const bool IsDead, const float DetachmentForce, const FGoreSystemHit BoneHit, float& NewBoneHealth, float& DamageApplied);

	/** Destroy all the additional skeletal components spawned from this system
	* >KeepDecals - Should skip blood decals?
	*/
	UFUNCTION(BlueprintCallable, Category = "GoreSystem")
	void RemoveAllSpawnedComponents(const bool KeepDecals);
	UFUNCTION(Server, Reliable)
	void RemoveAllSpawnedComponents_Server(const bool KeepDecals);
	UFUNCTION(NetMulticast, Reliable)
	void RemoveAllSpawnedComponents_Multi(const bool KeepDecals);

	/**[Internal/Legacy] Funtion called to spawn decal when a particle hits something (NO NIAGARA SUPPORT!!!)*/
	//UFUNCTION()
	//void SpawnDecalOnParticleHit(
	//	FName EventName,
	//	float EmitterTime,
	//	int32 ParticleTime,
	//	FVector Location,
	//	FVector Velocity,
	//	FVector Direction,
	//	FVector Normal,
	//	FName BoneName,
	//	UPhysicalMaterial* PhysMat);

	/**[HINT] Called when a bone has been broken*/
	UPROPERTY(BlueprintAssignable, Category = "GoreSystem")
	FOnBoneBroken On_BoneBroken;

	void ResetSlowMoTimer();

	FTimerHandle slowMoTimer;
};
