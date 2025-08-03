#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SXTorch.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class UParticleSystemComponent;
class URotatingMovementComponent;

UCLASS()
class SHOOTERX_API ASXTorch : public AActor
{
	GENERATED_BODY()
	
public:	
	ASXTorch();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

private:
    //UPROPERTY(VisibleAnywhere)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SXTorch", Meta = (AllowPrivateAccess))
    TObjectPtr<UBoxComponent> BoxComponent;

    //UPROPERTY(VisibleAnywhere)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SXTorch", Meta = (AllowPrivateAccess))
    TObjectPtr<UStaticMeshComponent> BodyStaticMeshComponent;

    //UPROPERTY(VisibleAnywhere)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SXTorch", Meta = (AllowPrivateAccess))
    TObjectPtr<UPointLightComponent> PointLightComponent;

    //UPROPERTY(VisibleAnywhere)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SXTorch", Meta = (AllowPrivateAccess))
    TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

    //UPROPERTY(EditAnywhere, Category = "SXTorch")
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "SXTorch", Meta = (AllowPrivateAccess))
    int32 ID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SXTorch", Meta = (AllowPrivateAccess))
    float RotationSpeed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SXTorch", meta = (AllowPrivateAccess))
    TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;
};
