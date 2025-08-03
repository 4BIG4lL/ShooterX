#include "Game/SXGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SXUnrealObject.h"

#include "Example/SXFlyable.h"
#include "Example/SXPigeon.h"
#include "Example/SXEagle.h"

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
	const FString SavedFileName(TEXT("SerializedPigeon76Data.bin"));
	FString AbsoluteFilePath = FPaths::Combine(*SavedDirectoryPath, *SavedFileName);
	FPaths::MakeStandardFilename(AbsoluteFilePath);

	TArray<uint8> BufferForWriter;
	FMemoryWriter MemoryWriterArchive(BufferForWriter);
	Pigeon76->Serialize(MemoryWriterArchive);

	TUniquePtr<FArchive> WriterArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*AbsoluteFilePath));
	if (WriterArchive != nullptr)
	{
		*WriterArchive << BufferForWriter;
		WriterArchive->Close();

		WriterArchive = nullptr;
	}

	TArray<uint8> BufferForReader;
	TUniquePtr<FArchive> ReaderArchive = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*AbsoluteFilePath));
	if (ReaderArchive != nullptr)
	{
		*ReaderArchive << BufferForReader;
		ReaderArchive->Close();

		ReaderArchive = nullptr;
	}

	FMemoryReader MemoryReaderArchive(BufferForReader);
	USXPigeon* ClonedPigeon76 = NewObject<USXPigeon>();
	ClonedPigeon76->Serialize(MemoryReaderArchive);
	UE_LOG(LogTemp, Log, TEXT("[ClonedPigeon76] Name: %s, ID: %d"), *ClonedPigeon76->GetPigeonName(), ClonedPigeon76->GetPigeonID());
}

void USXGameInstance::Shutdown()
{
    Super::Shutdown();
}