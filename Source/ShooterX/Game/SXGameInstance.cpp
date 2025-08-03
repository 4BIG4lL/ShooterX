#include "Game/SXGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SXUnrealObject.h"

USXGameInstance::USXGameInstance()
{
	// UE_LOG(LogTemp, Log, TEXT("USXGameInstance::USXGameInstance() has been called."));

    Name = TEXT("SXGameInstance's Class Default Object");
}

void USXGameInstance::Init()
{
    Super::Init();

    /*
    UE_LOG(LogTemp, Log, TEXT("USXGameInstance::Init() has been called."));

    UWorld* WorldInstance = GetWorld();
    if (IsValid(WorldInstance) == true)
    {
        UKismetSystemLibrary::PrintString(WorldInstance, TEXT("Init() has been called."));
    }

    checkf(IsValid(WorldInstance) == true, TEXT("WorldInstance is invalid."));
    UKismetSystemLibrary::PrintString(this, TEXT("Init() has been called."));
    

    UClass* CompiletimeClassInfo = StaticClass();
    UClass* RuntimeClassInfo = GetClass();

    checkf(CompiletimeClassInfo != RuntimeClassInfo, TEXT("CompiletimeClassInfo != RuntimeClassInfo"));

    UE_LOG(LogTemp, Log, TEXT("CompiletimeClassInfo->GetName(): %s"), *CompiletimeClassInfo->GetName());
    UE_LOG(LogTemp, Log, TEXT("RuntimeClassInfo->GetName(): %s"), *RuntimeClassInfo->GetName());

    Name = TEXT("SXGameInstance's Object");

    UE_LOG(LogTemp, Log, TEXT("USXGameInstance::Name %s"), *(RuntimeClassInfo->GetDefaultObject<USXGameInstance>()->Name));
    UE_LOG(LogTemp, Log, TEXT("USXGameInstance::Name %s"), *Name);
    */

    USXUnrealObject* USXObject01 = NewObject<USXUnrealObject>();
    // �𸮾��� �̷������� new Ű���带 �Ⱦ��� NewObject<>() API�� ����ؾ� ��.

    UE_LOG(LogTemp, Log, TEXT("USXObject01's Name: %s"), *USXObject01->GetObjectName());
    // �츮�� ������ Getter()

    FProperty* NameProperty = USXUnrealObject::StaticClass()->FindPropertyByName(TEXT("Name"));
    FString CompiletimeUSObjectName;
    if (nullptr != NameProperty)
    {
        NameProperty->GetValue_InContainer(USXObject01, &CompiletimeUSObjectName);
        UE_LOG(LogTemp, Log, TEXT("CompiletimeUSObjectName: %s"), *CompiletimeUSObjectName);
        // ���÷��� �ý����� Ȱ��
    }

    USXObject01->HelloUnreal();
    // ��� �Լ� ȣ��.

    UFunction* HelloUnrealFunction = USXObject01->GetClass()->FindFunctionByName(TEXT("HelloUnreal"));
    if (nullptr != HelloUnrealFunction)
    {
        USXObject01->ProcessEvent(HelloUnrealFunction, nullptr);
        // ���÷��� �ý����� Ȱ��
    }
}

void USXGameInstance::Shutdown()
{
    Super::Shutdown();
}