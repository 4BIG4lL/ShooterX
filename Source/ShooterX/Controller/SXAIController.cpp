#include "Controller/SXAIController.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

//const float ASXAIController::PatrolRepeatInterval(3.f);
const float ASXAIController::PatrolRadius(500.f);

int32 ASXAIController::ShowAIDebug(0);

FAutoConsoleVariableRef CVarShowAIDebug(
	TEXT("NXProject.ShowAIDebug"),
	ASXAIController::ShowAIDebug,
	TEXT(""),
	ECVF_Cheat
);

ASXAIController::ASXAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
	// BrainComponent는 AIController 클래스에 정의된 속성. UBrainComponent <- UBehaviorTreeComponent
}

const FName ASXAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName ASXAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName ASXAIController::TargetCharacterKey(TEXT("TargetCharacter"));

void ASXAIController::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &ThisClass::OnPatrolTimerElapsed, PatrolRepeatInterval, true);
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		BeginAI(ControlledPawn);
	}
}

void ASXAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);

	EndAI();

	Super::EndPlay(EndPlayReason);
}

void ASXAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent) == true)
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent) == true)
		{
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
			checkf(bRunSucceeded == true, TEXT("Fail to run behavior tree."));

			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());

			if (ShowAIDebug == 1)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI()")));
			}
		}
	}
}

void ASXAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();

		if (ShowAIDebug == 1)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("EndAI()")));
		}
	}
}