#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "DharmaShield.generated.h"

UENUM(BlueprintType)
enum class EDharmaType : uint8
{
    Protection,     // Divine protection and defense
    Reflection,     // Reflect incoming attacks
    Absorption,     // Absorb and convert energy
    Purification,   // Cleanse negative effects
    Enlightenment   // Divine shielding and wisdom
};

UCLASS()
class ECHOESOFCREATION_API ADharmaShield : public ABaseAbility
{
    GENERATED_BODY()

public:
    ADharmaShield();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific Dharma type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectDharmaType(EDharmaType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShieldRadius = 500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShieldDuration = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShieldStrength = 0.9f;

    // Currently selected Dharma type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EDharmaType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the Dharma shield effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateShieldEffect(const FVector& Center);

    // Apply Dharma effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyDharmaEffects(class AEchoesCharacter* Target);

    // Calculate shield intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateShieldIntensity(AEchoesCharacter* Target);

    // Timer handle for shield duration
    FTimerHandle ShieldTimer;
}; 