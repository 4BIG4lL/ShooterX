#include "Controller/SXPlayerController.h"

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
}