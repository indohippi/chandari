#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "PhilosophersStoneTransmutation.generated.h"

UENUM(BlueprintType)
enum class ETransmutationType : uint8
{
    LeadToGold,     // Basic transmutation
    SilverToMercury,// Elemental shift
    CopperToIron,   // Metal transformation
    TinToPlatinum,  // Noble metal creation
    Quicksilver     // Liquid metal manipulation
};

UCLASS()
class ECHOESOFCREATION_API APhilosophersStoneTransmutation : public ABaseAbility
{
    GENERATED_BODY()

public:
    APhilosophersStoneTransmutation();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific transmutation type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectTransmutationType(ETransmutationType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float TransmutationRadius = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float TransmutationDuration = 12.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float AlchemicalIntensity = 0.7f;

    // Currently selected transmutation type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    ETransmutationType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the transmutation field
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateTransmutationField(const FVector& Center);

    // Apply transmutation effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyTransmutationEffects(class AEchoesCharacter* Target);

    // Timer handle for transmutation duration
    FTimerHandle TransmutationTimer;
}; 