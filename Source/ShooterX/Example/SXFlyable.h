#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SXFlyable.generated.h"

UINTERFACE(MinimalAPI)
class USXFlyable : public UInterface
{
	GENERATED_BODY()
};

class SHOOTERX_API ISXFlyable
{
	GENERATED_BODY()

public:
	virtual void Fly() = 0;
};
