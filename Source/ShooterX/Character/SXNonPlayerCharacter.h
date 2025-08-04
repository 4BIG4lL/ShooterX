#pragma once

#include "CoreMinimal.h"
#include "Character/SXCharacterBase.h"
#include "SXNonPlayerCharacter.generated.h"

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

UCLASS()
class SHOOTERX_API ASXNonPlayerCharacter : public ASXCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;
	
public:
	ASXNonPlayerCharacter();

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginAttack();

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

public:
	bool bIsNowAttacking;

protected:
	FOnAttackMontageEnded OnAttackMontageEndedDelegate;
	// 애니메이션 몽타주 재생이 끝났을 때 호출할 델리게이트.
};
