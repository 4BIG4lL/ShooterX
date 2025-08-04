#include "Controller/SXPlayerController.h"

#include "UI/SXHUD.h"
#include "Game/SXPlayerState.h"
#include "Character/SXPlayerCharacter.h"
#include "Component/SXStatusComponent.h"

ASXPlayerController::ASXPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	// ��Ʈ�ѷ� ������ Tick�� ���ƾ� �Էµ� ó���� �� ����.
}

/*
void ASXPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("LeftRight"), this, &ThisClass::LeftRight);
}

void ASXPlayerController::LeftRight(float InAxisValue)
{
	UE_LOG(LogTemp, Error, TEXT("ASPlayerController::LeftRight(%.3f)"), InAxisValue);
}
*/

void ASXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if (IsValid(HUDWidgetClass) == true)
	{
		HUDWidget = CreateWidget<USXHUD>(this, HUDWidgetClass);
		if (IsValid(HUDWidget) == true)
		{
			HUDWidget->AddToViewport();

			ASXPlayerState* SXPlayerState = GetPlayerState<ASXPlayerState>();
			if (IsValid(SXPlayerState) == true)
			{
				HUDWidget->BindPlayerState(SXPlayerState);
			}

			ASXPlayerCharacter* PC = GetPawn<ASXPlayerCharacter>();
			if (IsValid(PC) == true)
			{
				USXStatusComponent* StatusComponent = PC->GetStatusComponent();
				if (IsValid(StatusComponent) == true)
				{
					HUDWidget->BindStatusComponent(StatusComponent);
				}
			}
		}
	}

	if (IsValid(CrosshairWidgetClass) == true)
	{
		UUserWidget* CrosshairWidgetInstance = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
		if (IsValid(CrosshairWidgetInstance) == true)
		{
			CrosshairWidgetInstance->AddToViewport(1);

			CrosshairWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}