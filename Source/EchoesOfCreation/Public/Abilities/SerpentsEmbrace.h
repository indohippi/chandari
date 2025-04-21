#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "SerpentsEmbrace.generated.h"

UENUM(BlueprintType)
enum class EEmbraceType : uint8
{
    Binding,    // Restrict movement and actions
    Knowledge,  // Share ancient wisdom
    Transformation, // Change form and essence
    Corruption, // Spread divine corruption
    Ascension   // Elevate to higher state
};

UCLASS()
class ECHOESOFCREATION_API ASerpentsEmbrace : public ABaseAbility
{
    GENERATED_BODY()

public:
    ASerpentsEmbrace();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific embrace type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectEmbraceType(EEmbraceType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float EmbraceRadius = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float EmbraceDuration = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float EmbraceIntensity = 0.85f;

    // Currently selected embrace type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EEmbraceType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the embrace effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateEmbraceEffect(const FVector& Center);

    // Apply embrace effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyEmbraceEffects(class AEchoesCharacter* Target);

    // Calculate embrace intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateEmbraceIntensity(AEchoesCharacter* Target);

    // Timer handle for embrace duration
    FTimerHandle EmbraceTimer;
}; 