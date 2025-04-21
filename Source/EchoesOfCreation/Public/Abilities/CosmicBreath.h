#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "CosmicBreath.generated.h"

UENUM(BlueprintType)
enum class ECosmicBreathType : uint8
{
    Solar,      // Radiant energy and light
    Lunar,      // Mystical and transformative
    Stellar,    // Cosmic power and creation
    Void,       // Dark energy and destruction
    Celestial   // Divine balance and harmony
};

UCLASS()
class ECHOESOFCREATION_API ACosmicBreath : public ABaseAbility
{
    GENERATED_BODY()

public:
    ACosmicBreath();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific cosmic breath type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectBreathType(ECosmicBreathType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BreathRadius = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BreathDuration = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float CosmicIntensity = 0.9f;

    // Currently selected breath type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    ECosmicBreathType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the cosmic breath effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateBreathEffect(const FVector& Center);

    // Apply breath effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyBreathEffects(class AEchoesCharacter* Target);

    // Timer handle for breath duration
    FTimerHandle BreathTimer;
}; 