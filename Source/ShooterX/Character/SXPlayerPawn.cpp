#include "Character/SXPlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ASXPlayerPawn::ASXPlayerPawn()
{
    PrimaryActorTick.bCanEverTick = false;

    float CharacterHalfHeight = 90.f;
    float CharacterRadius = 40.f;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    SetRootComponent(CapsuleComponent);
    CapsuleComponent->SetCapsuleHalfHeight(CharacterHalfHeight);
    CapsuleComponent->SetCapsuleRadius(CharacterRadius);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);
    FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
    FRotator PivotRotation(0.f, -90.f, 0.f);
    SkeletalMeshComponent->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassInfo(TEXT("/Script/Engine.AnimBlueprint'/Game/ShooterX/Animation/AnimationBlueprint/ABP_PlayerPawn.ABP_PlayerPawn_C'"));
    // �� ������Ʈ �н� ���, ���� ����ǥ ���� ���븸���ε� ������.
    //static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassInfo(TEXT("'/Game/ShooterX/Animation/AnimationBlueprint/ABP_PlayerPawn.ABP_PlayerPawn_C'")); 
    if (AnimInstanceClassInfo.Succeeded() == true)
    {
        SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        SkeletalMeshComponent->SetAnimInstanceClass(AnimInstanceClassInfo.Class);
    }

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 400.f;
    SpringArmComponent->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovementComponent"));

}

void ASXPlayerPawn::BeginPlay()
{
    Super::BeginPlay();

    /*
    SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);

    UAnimationAsset* AnimationSequenceAsset = LoadObject<UAnimationAsset>(SkeletalMeshComponent, TEXT("/Script/Engine.AnimSequence'/Game/LyraResource/Characters/Heroes/Mannequin/Animations/Locomotion/Unarmed/MM_Unarmed_Jog_Fwd.MM_Unarmed_Jog_Fwd'"));
    if (IsValid(AnimationSequenceAsset) == true)
    {
        SkeletalMeshComponent->PlayAnimation(AnimationSequenceAsset, true);
    }
    */
}

void ASXPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ThisClass::UpDown);
    PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ThisClass::LeftRight);
}

void ASXPlayerPawn::UpDown(float InAxisValue)
{
    AddMovementInput(GetActorForwardVector(), InAxisValue);
}

void ASXPlayerPawn::LeftRight(float InAxisValue)
{
    AddMovementInput(GetActorRightVector(), InAxisValue);
}
