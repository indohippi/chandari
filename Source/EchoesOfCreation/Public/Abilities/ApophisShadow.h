#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "ApophisShadow.generated.h"

UENUM(BlueprintType)
enum class EShadowType : uint8
{
    Chaos,      // Raw destructive power
    Darkness,   // Shadow manipulation
    Corruption, // Reality warping
    Void,       // Empty nothingness
    Serpent     // Apophis' essence
};

UCLASS()
class ECHOESOFCREATION_API AApophisShadow : public ABaseAbility
{
    GENERATED_BODY()

public:
    AApophisShadow();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific shadow type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectShadowType(EShadowType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShadowRadius = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ShadowDuration = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ChaosIntensity = 0.9f;

    // Currently selected shadow type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EShadowType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the shadow effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateShadowEffect(const FVector& Center);

    // Apply shadow effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyShadowEffects(class AEchoesCharacter* Target);

    // Calculate shadow intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateShadowIntensity(AEchoesCharacter* Target);

    // Timer handle for shadow duration
    FTimerHandle ShadowTimer;
}; 