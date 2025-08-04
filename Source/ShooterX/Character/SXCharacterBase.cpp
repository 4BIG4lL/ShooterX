#include "Character/SXCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterX.h"
#include "Animation/SXAnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Component/SXStatusComponent.h"

int32 ASXCharacterBase::ShowAttackMeleeDebug = 0;

FAutoConsoleVariableRef CVarShowAttackMeleeDebug(
	TEXT("SX.ShowAttackMeleeDebug"),
	ASXCharacterBase::ShowAttackMeleeDebug,
	TEXT(""),
	ECVF_Cheat
);

ASXCharacterBase::ASXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	float CharacterHalfHeight = 90.f;
	float CharacterRadius = 40.f;

	GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);

	FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);
	GetMesh()->SetCollisionProfileName(TEXT("SXCharacterMesh"));

	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//bIsDead = false;
	StatusComponent = CreateDefaultSubobject<USXStatusComponent>(TEXT("StatusComponent"));
}

void ASXCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	USXAnimInstance* AnimInstance = Cast<USXAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true)
	{
		AnimInstance->OnPostDead.AddDynamic(this, &ThisClass::HandleOnPostCharacterDead);
	}
}

void ASXCharacterBase::HandleOnPostCharacterDead()
{
	SetLifeSpan(0.1f);
}

void ASXCharacterBase::HandleOnCheckHit()
{
	//UKismetSystemLibrary::PrintString(this, TEXT("HandleOnCheckHit()"));

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + AttackMeleeRange * GetActorForwardVector(),
		FQuat::Identity,
		ECC_ATTACK,
		FCollisionShape::MakeSphere(AttackMeleeRadius),
		Params
	);

	if (true == bResult)
	{
		if (HitResults.IsEmpty() == false)
		{
			for (FHitResult HitResult : HitResults)
			{
				if (IsValid(HitResult.GetActor()) == true)
				{
					FDamageEvent DamageEvent;
					HitResult.GetActor()->TakeDamage(10.f, DamageEvent, GetController(), this);
					if (1 == ShowAttackMeleeDebug)
					{
						UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Hit Actor Name: %s"), *HitResult.GetActor()->GetName()));
					}
				}
			}
		}
	}

	if (1 == ShowAttackMeleeDebug)
	{
		FVector TraceVector = AttackMeleeRange * GetActorForwardVector();
		FVector Center = GetActorLocation() + TraceVector + GetActorUpVector() * 40.f;
		float HalfHeight = AttackMeleeRange * 0.5f + AttackMeleeRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
		FColor DrawColor = true == bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.f;

		DrawDebugCapsule(
			GetWorld(),
			Center,
			HalfHeight,
			AttackMeleeRadius,
			CapsuleRot,
			DrawColor,
			false,
			DebugLifeTime
		);
	}
}

float ASXCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//CurrentHP = FMath::Clamp(CurrentHP - FinalDamageAmount, 0.f, MaxHP);
	StatusComponent->ApplyDamage(FinalDamageAmount);

	//if (CurrentHP < KINDA_SMALL_NUMBER)
	if (StatusComponent->IsDead() == true)
	{
		//bIsDead = true;
		//CurrentHP = 0.f;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

	if (1 == ShowAttackMeleeDebug)
	{
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s [%.1f / %.1f]"), *GetName(), CurrentHP, MaxHP));
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s [%.1f / %.1f]"), *GetName(), StatusComponent->GetCurrentHP(), StatusComponent->GetMaxHP()));
	}

	return FinalDamageAmount;
}

void ASXCharacterBase::HandleOnCheckInputAttack()
{
	//UKismetSystemLibrary::PrintString(this, TEXT("HandleOnCheckInputAttack()"));

	USXAnimInstance* AnimInstance = Cast<USXAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	if (bIsAttackKeyPressed == true)
	{
		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);

		FName NextSectionName = *FString::Printf(TEXT("%s%02d"), *AttackAnimMontageSectionPrefix, CurrentComboCount);
		AnimInstance->Montage_JumpToSection(NextSectionName, AttackMeleeMontage);
		bIsAttackKeyPressed = false;
	}
}

void ASXCharacterBase::BeginAttack()
{
	USXAnimInstance* AnimInstance = Cast<USXAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	bIsNowAttacking = true;
	if (IsValid(AnimInstance) == true && IsValid(AttackMeleeMontage) == true && AnimInstance->Montage_IsPlaying(AttackMeleeMontage) == false)
	{
		AnimInstance->Montage_Play(AttackMeleeMontage);
	}

	CurrentComboCount = 1;

	if (OnMeleeAttackMontageEndedDelegate.IsBound() == false)
	{
		OnMeleeAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
		AnimInstance->Montage_SetEndDelegate(OnMeleeAttackMontageEndedDelegate, AttackMeleeMontage);
	}
}

void ASXCharacterBase::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	ensureMsgf(CurrentComboCount != 0, TEXT("CurrentComboCount == 0"));

	CurrentComboCount = 0;
	bIsAttackKeyPressed = false;
	bIsNowAttacking = false;
	// GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	if (OnMeleeAttackMontageEndedDelegate.IsBound() == true)
	{
		OnMeleeAttackMontageEndedDelegate.Unbind();
	}
}