#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "QiConduit.generated.h"

UENUM(BlueprintType)
enum class EQiType : uint8
{
    Flow,       // Smooth, continuous energy
    Focus,      // Concentrated, directed power
    Balance,    // Harmonious energy distribution
    Disrupt,    // Chaotic energy manipulation
    Dragon      // Pure draconic Qi
};

UCLASS()
class ECHOESOFCREATION_API AQiConduit : public ABaseAbility
{
    GENERATED_BODY()

public:
    AQiConduit();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific Qi type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectQiType(EQiType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ConduitRadius = 450.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ConduitDuration = 12.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float QiIntensity = 0.85f;

    // Currently selected Qi type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EQiType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the Qi conduit effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateConduitEffect(const FVector& Center);

    // Apply Qi effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyQiEffects(class AEchoesCharacter* Target);

    // Calculate Qi intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateQiIntensity(AEchoesCharacter* Target);

    // Timer handle for conduit duration
    FTimerHandle ConduitTimer;
}; 