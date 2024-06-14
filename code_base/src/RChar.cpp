// Fill out your copyright notice in the Description page of Project Settings.


#include "RChar.h"
#include "RWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponTypes.h"

//#include "GameFramework/CapsuleComponent.h"


ARChar::ARChar()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	//CameraBoom->bUsePawnControlRotation = true;
	//ViewCamera->bUsePawnControlRotation = false;

	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_CameraBoom->SetupAttachment(GetRootComponent());
	m_CameraBoom->bUsePawnControlRotation = true;

#if DEBUG
	m_CameraBoom->TargetArmLength = 200.f;
	MovementPtr->MaxWalkSpeed = 300.f;
#else
	m_CameraBoom->TargetArmLength = 400.f;
	MovementPtr->MaxWalkSpeed = 200.f;
#endif

	m_ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_ViewCamera->SetupAttachment(m_CameraBoom);

	m_AimSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Aim Sphere"));
	m_AimSphere->SetupAttachment(m_ViewCamera);
	m_AimSphere->SetVisibility(true);
	m_AimSphere->bHiddenInGame = true;

	m_InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	m_InteractionZone->SetupAttachment(GetRootComponent());
#if DEBUG
	m_InteractionZone->SetVisibility(true);
	m_InteractionZone->bHiddenInGame = false;
#endif
	m_SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	m_SpotLight->SetupAttachment(GetMesh(), "head");

//	m_EyeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeMesh"));
//	m_EyeMesh->SetupAttachment(GetMesh(), "head");
//	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
//	m_EyeMesh->SetStaticMesh(SphereMeshAsset.Object);

	m_Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	//m_PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	//m_PointLight->SetupAttachment(GetRootComponent());

	m_InteractionRayLenght = 600.f;
	m_SprintSpeedMultiplier = 2.f;

	m_IInterface = nullptr;

	m_TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ARChar::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("V9"));
}

void ARChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);

	TArray<AActor*> OverlapingActors;
	m_InteractionZone->GetOverlappingActors(OverlapingActors);
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Num of Actors = %i"), OverlapingActors.Num()));
	if (OverlapingActors.IsEmpty())
	{
		if (m_IInterface)
		{
			m_IInterface->HideInteractionWidget();
			m_IInterface = nullptr;
			m_NearestActor = nullptr;
		}
		return;
	}
	AActor* ClosetActor = OverlapingActors[0];
	for (auto curActor : OverlapingActors)
	{
		if (GetDistanceTo(curActor) < GetDistanceTo(ClosetActor))
		{
			ClosetActor = curActor;
		}
		if (m_IInterface)
		{
			m_IInterface->HideInteractionWidget();
		}
		m_IInterface = Cast<IInteractionInterface>(ClosetActor);
		m_NearestActor = Cast<AActor>(ClosetActor);
		if (m_IInterface)
		{
			m_IInterface->ShowInteractionWidget();
		}
	}
}

void ARChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Axis section
	// ===========================================================================================
	PlayerInputComponent->BindAxis(	FName(	"LookX"				), this, &ARChar::Turn			);
	PlayerInputComponent->BindAxis(	FName(	"LookY"				), this, &ARChar::LookUp		);
	PlayerInputComponent->BindAxis(	FName(	"MoveRightLeft"		), this, &ARChar::MoveRight		);
	PlayerInputComponent->BindAxis(	FName(	"MoveForwardBack"	), this, &ARChar::MoveForward	);
	// ===========================================================================================

	// Bind Actions section
	// =====================================================================================================
	PlayerInputComponent->BindAction(	"Interaction",	IE_Pressed,		this, &ARChar::OnInteract		  );
	PlayerInputComponent->BindAction(	"Sprint",		IE_Pressed,		this, &ARChar::Sprint			  );
	PlayerInputComponent->BindAction(	"Sprint",		IE_Released,	this, &ARChar::StopSprinting	  );
	PlayerInputComponent->BindAction(	"Normal",		IE_Pressed,		this, &ARChar::SetMStateNormal	  );
	PlayerInputComponent->BindAction(	"Scared",		IE_Pressed,		this, &ARChar::SetMStateScared	  );
	PlayerInputComponent->BindAction(	"Terrified",	IE_Pressed,		this, &ARChar::SetMStateTerrified );
	PlayerInputComponent->BindAction(	"CheckMental",	IE_Pressed,		this, &ARChar::CheckMentalState   );
	PlayerInputComponent->BindAction(	"Aim",			IE_Pressed,		this, &ARChar::AimButtonPressed	  );
	PlayerInputComponent->BindAction(	"Aim",			IE_Released,	this, &ARChar::AimButtonReleased  );
	PlayerInputComponent->BindAction(	"Fire",			IE_Pressed,		this, &ARChar::FireButtonPressed  );
	PlayerInputComponent->BindAction(	"Fire",			IE_Released,	this, &ARChar::FireButtonReleased );
	PlayerInputComponent->BindAction(	"Reload",		IE_Pressed,		this, &ARChar::ReloadButtonPressed);
	PlayerInputComponent->BindAction(   "Crouch",       IE_Pressed,     this, &ARChar::CrouchButtonPressed);
	PlayerInputComponent->BindAction(   "Jump",         IE_Pressed,		this,  &ARChar::Jump              );
	// =====================================================================================================

}

void ARChar::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (m_Combat)
	{
		m_Combat->m_Character = this;
	}
}

void ARChar::GetHit_Implementation(const FVector& impactPoint)
{
	Super::GetHit_Implementation(impactPoint);

	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Char get hitted!")));
}

float ARChar::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	if (attributes)
	{
		attributes->ReceiveDamage(damageAmount);

		if (!attributes->IsAlive()) Die();
	}

	return damageAmount;
}

void ARChar::PlayReloadMontage()
{
	if (!m_Combat || !m_Combat->m_EquippedWeapon) return;

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && reloadMontage)
	{
		animInstance->Montage_Play(reloadMontage);
		FName sectionName;
		switch (m_Combat->m_EquippedWeapon->GetWeaponType())
		{
			case EWeaponType::EWT_ASSAULTRIFLE:
				sectionName = FName("Rifle");
				break;
		}

		animInstance->Montage_JumpToSection(sectionName);
	}
}

bool ARChar::IsWeaponEquiped()
{
	return (m_Combat && m_Combat->m_EquippedWeapon);
}

bool ARChar::IsAiming()
{
	return (m_Combat && m_Combat->m_bIsAiming);
}

ECombatState ARChar::GetCombatState() const
{
	if (!m_Combat) return ECombatState::ECS_MAX;
	return m_Combat->combatState;
}

ARWeapon* ARChar::GetEquippedWeapon()
{
	if (m_Combat == nullptr) return nullptr;
	return m_Combat->m_EquippedWeapon;
}

FVector ARChar::GetHitTarget() const
{
	if (m_Combat == nullptr) return FVector();
	return m_Combat->m_HitTarget;
}

FVector ARChar::_getDirection(EAxis::Type _axis) const
{
	const FRotator _cntrlRot = GetControlRotation();
	const FRotator _yawRot(0.f, _cntrlRot.Yaw, 0.f);
	const FVector _dir = FRotationMatrix(_yawRot).GetUnitAxis(_axis);
	return _dir;
}

//void ARChar::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
//												AActor* OtherActor,
//									UPrimitiveComponent* OtherComp,
//											  int32 OtherBodyIndex,
//												   bool bFromSweep,
//									 const FHitResult& SweepResult)
//{
//	m_NearestActor = OtherActor;
//	m_IInterface = Cast<IInteractionInterface>(OtherActor);
//	if (m_IInterface)
//	{
//		m_IInterface->ShowInteractionWidget();
//	}
//}
//
//void ARChar::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (m_NearestActor)
//		m_NearestActor = nullptr;
//	if (m_IInterface)
//	{
//		m_IInterface->HideInteractionWidget();
//		m_IInterface = nullptr;
//	}
//}

void ARChar::OnInteract()
{
	if (m_IInterface)
	{
		switch (m_IInterface->GetInteractionType()) {
		case EInteractionTypes::EIT_Simple:
			m_IInterface->Interact();
			break;
		case EInteractionTypes::EIT_Weapon:
			m_Combat->EquipWeapon(Cast<ARWeapon>(m_NearestActor));
			break;
		default:
			// There should be a case for every shape type, so this should never happen.
			checkNoEntry();
		}

	}
}

void ARChar::TurnInPlace(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("m_AO_Yaw: %f"), m_AO_Yaw);
	if (m_AO_Yaw > 90.f)
	{
		m_TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (m_AO_Yaw < -90.f)
	{
		m_TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (m_TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		m_AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(m_AO_Yaw) < 15.f)
		{
			m_TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void ARChar::MoveForward(float value)
{
	if (Controller && (value != 0.f))
	{
		const FVector direction = _getDirection(EAxis::X);
		AddMovementInput(direction, value);
		//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Robot current speed = %f"), value));
	}
}

void ARChar::MoveRight(float value)
{
	if (Controller && (value != 0.f))
	{
		const FVector direction = _getDirection(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void ARChar::Turn(float value)
{
	AddControllerYawInput(value);
}

void ARChar::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ARChar::CheckMentalState()
{
	switch (m_MState) {
	case EMentalState::EMS_Normal:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green,
			FString::Printf(TEXT("Robot current mental state is normal")));
		break;
	case EMentalState::EMS_Scared:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow,
			FString::Printf(TEXT("Robot current mental state is scared")));
		break;
	case EMentalState::EMS_Terrified:
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red,
			FString::Printf(TEXT("Robot current mental state is terrified")));
		break;
	default:
		// There should be a case for every shape type, so this should never happen.
		checkNoEntry();
	}
}

void ARChar::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= m_SprintSpeedMultiplier;
}

void ARChar::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= m_SprintSpeedMultiplier;
}

void ARChar::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("Interacting")));
	FVector start = m_ViewCamera->GetComponentLocation();
	FVector end = start + m_ViewCamera->GetComponentRotation().Vector() * m_InteractionRayLenght;
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldStatic, params))
	{
		if (hitResult.GetActor())
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				FString::Printf(TEXT("Hitted Actor: %s"), *hitResult.GetActor()->GetName()));
	}
}

void ARChar::AimButtonPressed()
{
	if (m_Combat)
	{
		m_Combat->m_bIsAiming = true;
	}
}

void ARChar::AimButtonReleased()
{
	if (m_Combat)
	{
		m_Combat->m_bIsAiming = false;
	}
}

void ARChar::FireButtonPressed()
{
	if (m_Combat)
	{
		m_Combat->FireButtonPressed(true);
	}
}

void ARChar::FireButtonReleased()
{
	if (m_Combat)
	{
		m_Combat->FireButtonPressed(false);
	}
}

void ARChar::ReloadButtonPressed()
{
	if (m_Combat)
	{
		m_Combat->ReloadButtonPressed();
	}
}

void ARChar::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}

}

void ARChar::AimOffset(float DeltaTime)
{
	if (m_Combat && m_Combat->m_EquippedWeapon == nullptr) return;
	FVector Vel = GetVelocity();
	Vel.Z = 0.f;
	float m_speed = Vel.Size();
	bool m_bIsInAir = GetCharacterMovement()->IsFalling();

	if (m_speed == 0.f && !m_bIsInAir) //standing still and not jumping
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		m_AO_Yaw = DeltaAimRotation.Yaw;
		if (m_TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = m_AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (m_speed > 0.f || m_bIsInAir) // running or jumping
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		m_AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		m_TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	m_AO_Pitch = GetBaseAimRotation().Pitch;
}

void ARChar::Die_Implementation()
{
	Super::Die_Implementation();

	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Char died!")));

	//playerDiedDelegate.Broadcast();

	ARGameModeBase* GameMode = Cast<ARGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->HandlePlayerDeath();
}

//void ARChar::Die()
//{
//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Char dead!")));
//}


void ARChar::SetMStateNormal()
{
	if (m_MState != EMentalState::EMS_Normal) m_MState = EMentalState::EMS_Normal;
}

void ARChar::SetMStateScared()
{
	if (m_MState != EMentalState::EMS_Scared) m_MState = EMentalState::EMS_Scared;
}

void ARChar::SetMStateTerrified()
{
	if (m_MState != EMentalState::EMS_Terrified) m_MState = EMentalState::EMS_Terrified;
}
