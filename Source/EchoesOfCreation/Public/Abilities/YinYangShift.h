#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "YinYangShift.generated.h"

UENUM(BlueprintType)
enum class EShiftType : uint8
{
    YinToYang,  // Transform dark to light
    YangToYin,  // Transform light to dark
    Balance,    // Maintain perfect equilibrium
    Duality,    // Simultaneous existence of both
    Dragon      // Channel both through draconic power
};

UCLASS()
class ECHOESOFCREATION_API AYinYangShift : public ABaseAbility
{
    GENERATED_BODY()

public:
    AYinYangShift();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific shift type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectShiftType(EShiftType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShiftRadius = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShiftDuration = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShiftIntensity = 0.8f;

    // Currently selected shift type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EShiftType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the shift effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateShiftEffect(const FVector& Center);

    // Apply shift effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyShiftEffects(class AEchoesCharacter* Target);

    // Calculate shift intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateShiftIntensity(AEchoesCharacter* Target);

    // Timer handle for shift duration
    FTimerHandle ShiftTimer;
}; 