#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Resonance/ResonanceTypes.h"
#include "ResonanceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResonanceGenerated, const FResonanceData&, ResonanceData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResonanceEffectReceived, const FResonanceEffect&, ResonanceEffect);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOESOFCREATION_API UResonanceComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UResonanceComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Generate resonance of a specific type
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void GenerateResonance(EResonanceType Type, float Intensity, float Duration = 5.0f);

    // Get current resonance effects
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    FResonanceEffect GetCurrentResonanceEffect() const { return CurrentResonanceEffect; }

    // Resonance generation delegates
    UPROPERTY(BlueprintAssignable, Category = "Resonance")
    FOnResonanceGenerated OnResonanceGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Resonance")
    FOnResonanceEffectReceived OnResonanceEffectReceived;

protected:
    // Current resonance effect
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonance")
    FResonanceEffect CurrentResonanceEffect;

    // Resonance resistance
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float ResonanceResistance;

    // Resonance sensitivity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float ResonanceSensitivity;

    // Update resonance effects
    UFUNCTION()
    void UpdateResonanceEffects(float DeltaTime);

    // Handle resonance effects
    void HandleResonanceEffect(const FResonanceEffect& Effect);

    // Timer handle for resonance updates
    FTimerHandle ResonanceUpdateTimer;
}; 