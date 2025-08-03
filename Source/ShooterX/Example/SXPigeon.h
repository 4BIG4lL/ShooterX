#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Example/SXFlyable.h"
#include "SXPigeon.generated.h"

USTRUCT()
struct FSXPigeonData
{
	GENERATED_BODY()

public:
	FSXPigeonData()
	{

	}

	FSXPigeonData(const FString& InName, int32 InID)
		: Name(InName)
		, ID(InID)
	{

	}

	friend FArchive& operator<<(FArchive& InArchive, FSXPigeonData& InPigeonData)
	{
		InArchive << InPigeonData.Name;
		InArchive << InPigeonData.ID;
		return InArchive;
	}
	
public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 ID;
};

UCLASS()
class SHOOTERX_API USXPigeon 
	: public UObject
	, public ISXFlyable
{
	GENERATED_BODY()
	
public:
	USXPigeon();

	virtual void Fly() override;

	const FString& GetPigeonName() const
	{
		return Name;
	}
	void SetPigeonName(const FString& InName)
	{
		Name = InName;
	}

	int32 GetPigeonID() const
	{
		return ID;
	}
	void SetPigeonID(const int32& InID)
	{
		ID = InID;
	}

	virtual void Serialize(FArchive& Ar) override;

private:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 ID;

};
