#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SXHUD.generated.h"

class USXStatusComponent;
class ASXPlayerState;
class UTextBlock;
class UUW_HPBar;

/**
 *
 */
UCLASS()
class SHOOTERX_API USXHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindStatusComponent(USXStatusComponent* InStatusComponent);

	void BindPlayerState(ASXPlayerState* InPlayerState);

protected:
	UFUNCTION()
	void OnKillCountChanged(int32 InKillCount);

protected:
	TObjectPtr<USXStatusComponent> StatusComponent;

	TObjectPtr<ASXPlayerState> PlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> KillCountText;
	// BindWidget Ű���带 ���ؼ� �ϵ��ڵ����� �Ӽ��� ������ ���ε� ���� �ʾƵ� ��.
	// ��� �Ӽ��� �̸��� ���� �������Ʈ �� ������ �̸��� ���̾���.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUW_HPBar> HPBar;

};