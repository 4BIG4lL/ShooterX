#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SXCharacterBase.generated.h"

UCLASS()
class SHOOTERX_API ASXCharacterBase : public ACharacter
{
	GENERATED_BODY()

#pragma region Override ACharacter

public:
	ASXCharacterBase();

protected:
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Attack

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMeleeMontage;

	FString AttackAnimMontageSectionPrefix = FString(TEXT("Attack"));

	int32 MaxComboCount = 3;

	int32 CurrentComboCount = 0;

	bool bIsNowAttacking = false;

	bool bIsAttackKeyPressed = false;

	FOnMontageEnded OnMeleeAttackMontageEndedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackMeleeRange = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackMeleeRadius = 20.f;

public:
	UFUNCTION()
	void HandleOnCheckHit();

	UFUNCTION()
	void HandleOnCheckInputAttack();

	virtual void BeginAttack();

	static int32 ShowAttackMeleeDebug;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

#pragma endregion

#pragma region HP

public:
	float GetMaxHP() const { return MaxHP; }

	float GetCurrentHP() const { return CurrentHP; }

	void SetMaxHP(float InMaxHP) { MaxHP = InMaxHP; }

	void SetCurrentHP(float InCurrentHP) { CurrentHP = InCurrentHP; }

	bool IsDead() const { return bIsDead; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHP = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsDead : 1;

	UFUNCTION()
	virtual void HandleOnPostCharacterDead();

#pragma endregion

};
