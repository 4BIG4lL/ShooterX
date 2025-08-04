#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SXPlayerController.generated.h"

class USXHUD;

UCLASS()
class SHOOTERX_API ASXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASXPlayerController();

	USXHUD* GetHUDWidget() const { return HUDWidget; };

protected:
	// virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	UPROPERTY();
	TObjectPtr<USXHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess));
	TSubclassOf<USXHUD> HUDWidgetClass;
};
