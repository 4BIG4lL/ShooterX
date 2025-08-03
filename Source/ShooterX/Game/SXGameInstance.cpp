#include "Game/SXGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SXUnrealObject.h"

#include "Example/SXFlyable.h"
#include "Example/SXPigeon.h"
#include "Example/SXEagle.h"

#include "JsonObjectConverter.h"
#include "UObject/SavePackage.h"

USXGameInstance::USXGameInstance()
{
	
}

void USXGameInstance::Init()
{
	Super::Init();

	USXPigeon* Pigeon76 = NewObject<USXPigeon>();
	Pigeon76->SetPigeonName(TEXT("Pigeon76"));
	Pigeon76->SetPigeonID(76);
	UE_LOG(LogTemp, Log, TEXT("[Pigeon76] Name: %s, ID: %d"), *Pigeon76->GetPigeonName(), Pigeon76->GetPigeonID());

	const FString SavedDirectoryPath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	const FString SavedFileName(TEXT("SerializedPigeon76JsonData.txt"));
	FString AbsoluteFilePath = FPaths::Combine(*SavedDirectoryPath, *SavedFileName);
	FPaths::MakeStandardFilename(AbsoluteFilePath);

	TSharedRef<FJsonObject> Pigeon76JsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(Pigeon76->GetClass(), Pigeon76, Pigeon76JsonObject);

	FString WritedJsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriterArchive = TJsonWriterFactory<TCHAR>::Create(&WritedJsonString);
	if (FJsonSerializer::Serialize(Pigeon76JsonObject, JsonWriterArchive) == true)
	{
		FFileHelper::SaveStringToFile(WritedJsonString, *AbsoluteFilePath);
	}

	FString ReadedJsonString;
	FFileHelper::LoadFileToString(ReadedJsonString, *AbsoluteFilePath);
	TSharedRef<TJsonReader<TCHAR>> JsonReaderArchive = TJsonReaderFactory<TCHAR>::Create(ReadedJsonString);

	USXPigeon* ClonedPigeon76 = NewObject<USXPigeon>();

	TSharedPtr<FJsonObject> ClonedPigeon76JsonObject = nullptr;
	if (FJsonSerializer::Deserialize(JsonReaderArchive, ClonedPigeon76JsonObject) == true)
	{
		if (FJsonObjectConverter::JsonObjectToUStruct(ClonedPigeon76JsonObject.ToSharedRef(), ClonedPigeon76->GetClass(), ClonedPigeon76) == true)
		{
			UE_LOG(LogTemp, Log, TEXT("[ClonedPigeon76] Name: %s, ID: %d"), *ClonedPigeon76->GetPigeonName(), ClonedPigeon76->GetPigeonID());
		}
	}
}

void USXGameInstance::Shutdown()
{
    Super::Shutdown();
}