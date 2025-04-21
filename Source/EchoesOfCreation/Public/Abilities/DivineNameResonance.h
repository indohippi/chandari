#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "DivineNameResonance.generated.h"

UENUM(BlueprintType)
enum class EDivineName : uint8
{
    El,         // Mercy and compassion
    Elohim,     // Justice and power
    YHVH,       // The ineffable name
    Adonai,     // Lordship and sovereignty
    Shaddai     // Almighty and protection
};

UCLASS()
class ECHOESOFCREATION_API ADivineNameResonance : public ABaseAbility
{
    GENERATED_BODY()

public:
    ADivineNameResonance();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific divine name
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectDivineName(EDivineName Name);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ResonanceRadius = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ResonanceDuration = 8.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ResonanceIntensity = 0.7f;

    // Currently selected divine name
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EDivineName SelectedName;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create resonance field
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateResonanceField(const FVector& Center);

    // Apply resonance effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyResonanceEffects(class AEchoesCharacter* Target);

    // Timer handle for resonance duration
    FTimerHandle ResonanceTimer;
}; 