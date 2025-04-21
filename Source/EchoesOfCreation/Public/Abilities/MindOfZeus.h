#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "MindOfZeus.generated.h"

UENUM(BlueprintType)
enum class EZeusAspect : uint8
{
    Thunder,    // Lightning and power
    Wisdom,     // Divine knowledge
    Justice,    // Divine judgment
    Authority,  // Divine rule
    Storm       // Weather control
};

UCLASS()
class ECHOESOFCREATION_API AMindOfZeus : public ABaseAbility
{
    GENERATED_BODY()

public:
    AMindOfZeus();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific aspect of Zeus
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectZeusAspect(EZeusAspect Aspect);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float AspectRadius = 450.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float AspectDuration = 18.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float DivinePower = 0.8f;

    // Currently selected aspect
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EZeusAspect SelectedAspect;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the divine field
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateDivineField(const FVector& Center);

    // Apply aspect effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyAspectEffects(class AEchoesCharacter* Target);

    // Timer handle for aspect duration
    FTimerHandle AspectTimer;
}; 