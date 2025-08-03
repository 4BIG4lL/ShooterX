#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SXPlayerController.generated.h"

UCLASS()
class SHOOTERX_API ASXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASXPlayerController();

protected:
	// virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

// private:
	// void LeftRight(float InAxisValue);
};
