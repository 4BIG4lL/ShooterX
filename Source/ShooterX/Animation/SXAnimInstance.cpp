#include "Animation/SXAnimInstance.h"
#include "Character/SXCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/SXNonPlayerCharacter.h"

void USXAnimInstance::NativeInitializeAnimation()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn) == true)
	{
		OwnerCharacter = Cast<ASXCharacterBase>(OwnerPawn);
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void USXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(OwnerCharacter) == true && IsValid(OwnerCharacterMovement) == true)
	{
		Velocity = OwnerCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

		float GroundAcceleration = UKismetMathLibrary::VSizeXY(OwnerCharacterMovement->GetCurrentAcceleration());
		bool bIsAccelerated = FMath::IsNearlyZero(GroundAcceleration) == false;
		bShouldMove = (KINDA_SMALL_NUMBER < GroundSpeed) && (bIsAccelerated == true);
		if (ASXNonPlayerCharacter* OwnerNPC = Cast<ASXNonPlayerCharacter>(OwnerCharacter))
		{
			bShouldMove = KINDA_SMALL_NUMBER < GroundSpeed;
		}
		bIsFalling = OwnerCharacterMovement->IsFalling();
		bIsDead = OwnerCharacter->IsDead();
	}
}

void USXAnimInstance::AnimNotify_CheckHit()
{
	if (OnCheckHit.IsBound() == true)
	{
		OnCheckHit.Broadcast();
	}
}

void USXAnimInstance::AnimNotify_PostDead()
{
	if (OnPostDead.IsBound() == true)
	{
		OnPostDead.Broadcast();
	}
}