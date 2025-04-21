#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "DragonsBreath.generated.h"

UENUM(BlueprintType)
enum class EBreathType : uint8
{
    Yin,        // Dark, cold, passive energy
    Yang,       // Light, hot, active energy
    Balance,    // Perfect harmony of both
    Chaos,      // Unstable mix of energies
    Dragon      // Pure draconic essence
};

UCLASS()
class ECHOESOFCREATION_API ADragonsBreath : public ABaseAbility
{
    GENERATED_BODY()

public:
    ADragonsBreath();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific breath type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectBreathType(EBreathType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BreathRadius = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BreathDuration = 8.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BreathIntensity = 0.75f;

    // Currently selected breath type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EBreathType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the breath effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateBreathEffect(const FVector& Center);

    // Apply breath effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyBreathEffects(class AEchoesCharacter* Target);

    // Calculate breath intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateBreathIntensity(AEchoesCharacter* Target);

    // Timer handle for breath duration
    FTimerHandle BreathTimer;
}; 