#pragma once

#include "CoreMinimal.h"
#include "Character/SXCharacterBase.h"
#include "SXNonPlayerCharacter.generated.h"

class USXHPTextWidgetComponent;
class UUW_HPText;

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

UCLASS()
class SHOOTERX_API ASXNonPlayerCharacter : public ASXCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;
	
public:
	ASXNonPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetHPTextWidget(UUW_HPText* InHPTextWidget);
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginAttack();

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USXHPTextWidgetComponent> HPTextWidgetComponent;

public:
	bool bIsNowAttacking;

protected:
	FOnAttackMontageEnded OnAttackMontageEndedDelegate;
	// �ִϸ��̼� ��Ÿ�� ����� ������ �� ȣ���� ��������Ʈ.
};
