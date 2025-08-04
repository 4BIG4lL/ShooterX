#include "Character/SXPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/SXInputConfig.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/SXAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "ShooterXPlayGround/SXPlayerCharacterMaterialManager.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Item/SXWeapon.h"

#include "Kismet/KismetMathLibrary.h"
#include "Component/SXPickupComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "ShooterX.h"

ASXPlayerCharacter::ASXPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 25.f, 25.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetAutoActivate(false);

	//const USXPlayerCharacterMaterialManager* CDO = GetDefault<USXPlayerCharacterMaterialManager>();
	//if (0 < CDO->PlayerCharacterMeshMaterialPaths.Num())
	//{
	//	for (FSoftObjectPath PlayerCharacterMeshPath : CDO->PlayerCharacterMeshMaterialPaths)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Object Path: % s"), *(PlayerCharacterMeshPath.ToString()));
	//		// 어떻게 ASXPlayerCharacter() 생성자에서 USXPlayerCharacterMaterialManager 클래스의 CDO를 참조할 수 있었을까?
	//	}
	//}
	TimeBetweenFire = 60.f / FirePerMinute;
}

void ASXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController) == true)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (IsValid(Subsystem) == true)
		{
			Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
		}
	}

	const USXPlayerCharacterMaterialManager* CDO = GetDefault<USXPlayerCharacterMaterialManager>();
	int32 RandomIndex = FMath::RandRange(0, (CDO->PlayerCharacterMeshMaterialPaths.Num() / 2) - 1);
	CurrentPlayerCharacterMeshMaterialPath01 = CDO->PlayerCharacterMeshMaterialPaths[RandomIndex];
	CurrentPlayerCharacterMeshMaterialPath02 = CDO->PlayerCharacterMeshMaterialPaths[RandomIndex + 1];
	AssetStreamableHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		{ CurrentPlayerCharacterMeshMaterialPath01, CurrentPlayerCharacterMeshMaterialPath02 },
		FStreamableDelegate::CreateLambda([this]() -> void
	{
		AssetStreamableHandle->ReleaseHandle();
		TSoftObjectPtr<UMaterialInstance> LoadedMaterialInstance01(CurrentPlayerCharacterMeshMaterialPath01);
		TSoftObjectPtr<UMaterialInstance> LoadedMaterialInstance02(CurrentPlayerCharacterMeshMaterialPath02);
		if (LoadedMaterialInstance01.IsValid() == true && LoadedMaterialInstance02.IsValid() == true)
		{
			GetMesh()->SetMaterial(1, LoadedMaterialInstance01.Get());
			GetMesh()->SetMaterial(0, LoadedMaterialInstance02.Get());
		}
	}
		)
	);
}

void ASXPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);
}

void ASXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent) == true)
	{
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->AttackMelee, ETriggerEvent::Started, this, &ThisClass::InputAttackMelee);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->AttackRanged, ETriggerEvent::Started, this, &ThisClass::InputAttackRanged);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->ToggleSelector, ETriggerEvent::Started, this, &ThisClass::InputToggleSelector);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->AttackRanged, ETriggerEvent::Started, this, &ThisClass::InputStartFullAutoFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->AttackRanged, ETriggerEvent::Completed, this, &ThisClass::InputStopFullAutoFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->IronSight, ETriggerEvent::Started, this, &ThisClass::InputStartIronSight);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->IronSight, ETriggerEvent::Completed, this, &ThisClass::InputEndIronSight);
	}
}

void ASXPlayerCharacter::InputMove(const FInputActionValue& InValue)
{
	FVector2D MovementVector = InValue.Get<FVector2D>();

	const FRotator ControlRotation = GetController()->GetControlRotation();
	const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.X);
	AddMovementInput(RightVector, MovementVector.Y);
}

void ASXPlayerCharacter::InputLook(const FInputActionValue& InValue)
{
	FVector2D LookVector = InValue.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ASXPlayerCharacter::InputAttackMelee(const FInputActionValue& InValue)
{
	if (GetCharacterMovement()->IsFalling() == true)
	{
		return;
	}

	/*
	USXAnimInstance* AnimInstance = Cast<USXAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true && IsValid(AttackMeleeMontage) == true && AnimInstance->Montage_IsPlaying(AttackMeleeMontage) == false)
	{
		AnimInstance->Montage_Play(AttackMeleeMontage);
	}
	*/

	if (0 == CurrentComboCount)
	{
		BeginAttack();
	}
	else
	{
		ensure(FMath::IsWithinInclusive<int32>(CurrentComboCount, 1, MaxComboCount));
		bIsAttackKeyPressed = true;
	}
}

void ASXPlayerCharacter::InputAttackRanged(const FInputActionValue& InValue)
{
	if (0.f < GetCharacterMovement()->Velocity.Size())
	{
		return;
	}

	if (IsValid(CurrentWeapon) == false)
	{
		return;
	}

	if (IsValid(GetCurrentWeaponAttackAnimMontage()) == false)
	{
		return;
	}

	/*
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true)
	{
		if (AnimInstance->Montage_IsPlaying(GetCurrentWeaponAttackAnimMontage()) == false)
		{
			AnimInstance->Montage_Play(GetCurrentWeaponAttackAnimMontage());
		}
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetController());
	if (IsValid(AttackRangedCameraShake) == true && IsValid(OwnerPlayerController) == true)
	{
		OwnerPlayerController->ClientStartCameraShake(AttackRangedCameraShake);
	}
	*/

	// TryFire();
	if (false == bIsFullAutoFire)
	{
		TryFire();
	}
}

void ASXPlayerCharacter::TryFire()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (IsValid(PlayerController) == true)
	{
#pragma region CaculateTargetTransform
		float FocalDistance = 400.f;
		FVector FocalLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector AimDirectionFromCamera = CameraRotation.Vector().GetSafeNormal();
		FocalLocation = CameraLocation + (AimDirectionFromCamera * FocalDistance);

		FVector WeaponMuzzleLocation = CurrentWeapon->GetPickupComponent()->GetSocketLocation(TEXT("MuzzleFlash"));
		FVector FinalFocalLocation = FocalLocation + (((WeaponMuzzleLocation - FocalLocation) | AimDirectionFromCamera) * AimDirectionFromCamera);

		FTransform TargetTransform = FTransform(CameraRotation, FinalFocalLocation);

		if (1 == ShowAttackRangedDebug)
		{
			DrawDebugSphere(GetWorld(), WeaponMuzzleLocation, 2.f, 16, FColor::Red, false, 60.f);

			DrawDebugSphere(GetWorld(), CameraLocation, 2.f, 16, FColor::Yellow, false, 60.f);

			DrawDebugSphere(GetWorld(), FinalFocalLocation, 2.f, 16, FColor::Magenta, false, 60.f);

			// (WeaponLoc - FocalLoc)
			DrawDebugLine(GetWorld(), FocalLocation, WeaponMuzzleLocation, FColor::Yellow, false, 60.f, 0, 2.f);

			// AimDir
			DrawDebugLine(GetWorld(), CameraLocation, FinalFocalLocation, FColor::Blue, false, 60.f, 0, 2.f);

			// Project Direction Line
			DrawDebugLine(GetWorld(), WeaponMuzzleLocation, FinalFocalLocation, FColor::Red, false, 60.f, 0, 2.f);
		}

#pragma endregion

#pragma region PerformLineTracing

		FVector BulletDirection = TargetTransform.GetUnitAxis(EAxis::X);
		FVector StartLocation = WeaponMuzzleLocation;
		FVector EndLocation = TargetTransform.GetLocation() + BulletDirection * CurrentWeapon->GetMaxAttackRange();

		FHitResult HitResult;
		FCollisionQueryParams TraceParams(NAME_None, false, this);
		TraceParams.AddIgnoredActor(CurrentWeapon);

		bool IsCollided = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_ATTACK, TraceParams);
		if (IsCollided == false)
		{
			HitResult.TraceStart = StartLocation;
			HitResult.TraceEnd = EndLocation;
		}

		if (2 == ShowAttackRangedDebug)
		{
			if (IsCollided == true)
			{
				DrawDebugSphere(GetWorld(), StartLocation, 2.f, 16, FColor::Red, false, 60.f);

				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.f, 16, FColor::Green, false, 60.f);

				DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 60.f, 0, 2.f);
			}
			else
			{
				DrawDebugSphere(GetWorld(), StartLocation, 2.f, 16, FColor::Red, false, 60.f);

				DrawDebugSphere(GetWorld(), EndLocation, 2.f, 16, FColor::Green, false, 60.f);

				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 60.f, 0, 2.f);
			}
		}

#pragma endregion

		if (IsCollided == true)
		{
			ASXCharacterBase* HittedCharacter = Cast<ASXCharacterBase>(HitResult.GetActor());
			if (IsValid(HittedCharacter) == true)
			{
				FDamageEvent DamageEvent;
				HittedCharacter->TakeDamage(10.f, DamageEvent, GetController(), this);
			}
		}

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance) == true)
		{
			if (AnimInstance->Montage_IsPlaying(GetCurrentWeaponAttackAnimMontage()) == false)
			{
				AnimInstance->Montage_Play(GetCurrentWeaponAttackAnimMontage());
			}
		}

		if (IsValid(AttackRangedCameraShake) == true)
		{
			PlayerController->ClientStartCameraShake(AttackRangedCameraShake);
		}
	}
}

void ASXPlayerCharacter::InputToggleSelector(const FInputActionValue& InValue)
{
	bIsFullAutoFire = !bIsFullAutoFire;
}

void ASXPlayerCharacter::InputStartFullAutoFire(const FInputActionValue& InValue)
{
	if (true == bIsFullAutoFire)
	{
		GetWorldTimerManager().SetTimer(FullAutoTimerHandle, this, &ThisClass::TryFire, TimeBetweenFire, true);
	}
}

void ASXPlayerCharacter::InputStopFullAutoFire(const FInputActionValue& InValue)
{
	GetWorldTimerManager().ClearTimer(FullAutoTimerHandle);
}

void ASXPlayerCharacter::InputStartIronSight(const FInputActionValue& InValue)
{
	TargetFOV = 45.f;
}

void ASXPlayerCharacter::InputEndIronSight(const FInputActionValue& InValue)
{
	TargetFOV = 70.f;
}