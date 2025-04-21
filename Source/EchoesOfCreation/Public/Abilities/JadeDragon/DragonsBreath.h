#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "DragonsBreath.generated.h"

UENUM(BlueprintType)
enum class EBreathType : uint8
{
    CelestialFire UMETA(DisplayName = "Celestial Fire"),
    AbyssalFrost UMETA(DisplayName = "Abyssal Frost"),
    EternalStorm UMETA(DisplayName = "Eternal Storm")
};

UCLASS()
class ECHOESOFCREATION_API ADragonsBreath : public AActor
{
    GENERATED_BODY()

public:
    ADragonsBreath();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeBreath(EBreathType BreathType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateBreath();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateBreath();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void TargetBreath(FVector TargetLocation);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    EBreathType CurrentBreathType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    FVector CurrentTargetLocation;

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathTargeted(FVector TargetLocation);

private:
    void ApplyBreathEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateBreathState(float DeltaTime);
    void CheckAffectedActors();
}; 