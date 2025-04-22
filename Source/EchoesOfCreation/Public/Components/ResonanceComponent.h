#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResonanceComponent.generated.h"

UENUM(BlueprintType)
enum class EResonanceType : uint8
{
    Faith,
    Doubt,
    Curiosity
};

USTRUCT(BlueprintType)
struct FResonanceState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float Faith = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float Doubt = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float Curiosity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float TotalResonance = 0.0f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOESOFCREATION_API UResonanceComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UResonanceComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Resonance Management
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void AddResonance(EResonanceType Type, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void RemoveResonance(EResonanceType Type, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    float GetResonance(EResonanceType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    float GetTotalResonance() const;

    // Resonance Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance")
    void OnResonanceChanged(EResonanceType Type, float NewValue);

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance")
    void OnResonanceThresholdReached(EResonanceType Type);

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance")
    void OnEchoManifestation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance")
    void OnEnvironmentDistortion();

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance")
    void OnCorruption();

private:
    UPROPERTY(EditAnywhere, Category = "Resonance")
    FResonanceState ResonanceState;

    UPROPERTY(EditAnywhere, Category = "Resonance")
    float ResonanceDecayRate = 0.1f;

    UPROPERTY(EditAnywhere, Category = "Resonance")
    float ManifestationThreshold = 50.0f;

    UPROPERTY(EditAnywhere, Category = "Resonance")
    float DistortionThreshold = 30.0f;

    UPROPERTY(EditAnywhere, Category = "Resonance")
    float CorruptionThreshold = 70.0f;

    void UpdateResonance(float DeltaTime);
    void CheckResonanceEffects();
    void ApplyResonanceDecay(float DeltaTime);
}; 