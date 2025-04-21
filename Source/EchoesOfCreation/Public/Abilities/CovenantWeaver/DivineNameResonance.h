#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "DivineNameResonance.generated.h"

UENUM(BlueprintType)
enum class EResonanceType : uint8
{
    Divine UMETA(DisplayName = "Divine"),
    Celestial UMETA(DisplayName = "Celestial"),
    Sacred UMETA(DisplayName = "Sacred")
};

UCLASS()
class ECHOESOFCREATION_API ADivineNameResonance : public AActor
{
    GENERATED_BODY()

public:
    ADivineNameResonance();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeResonance(EResonanceType ResonanceType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateResonance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateResonance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void TargetResonance(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    EResonanceType CurrentResonanceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonancePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonanceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonanceRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance Effects")
    void OnResonanceActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance Effects")
    void OnResonanceDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance Effects")
    void OnTargetResonated(AActor* Target, float ResonanceStrength);

private:
    void ApplyResonanceEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateResonanceState(float DeltaTime);
    bool CheckTargetValidity(AActor* Target);
    float CalculateResonanceStrength(AActor* Target);
    void AmplifyDivineName(AActor* Target, float DeltaTime);
}; 