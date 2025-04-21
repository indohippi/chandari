#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "PranaConduit.generated.h"

UENUM(BlueprintType)
enum class EPranaType : uint8
{
    Vital,      // Life force and healing
    Spiritual,  // Spiritual energy and enlightenment
    Cosmic,     // Cosmic energy and power
    Karmic,     // Karmic energy and balance
    Divine      // Divine energy and transcendence
};

UCLASS()
class ECHOESOFCREATION_API APranaConduit : public ABaseAbility
{
    GENERATED_BODY()

public:
    APranaConduit();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific prana type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectPranaType(EPranaType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ConduitRadius = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ConduitDuration = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float PranaIntensity = 0.8f;

    // Currently selected prana type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EPranaType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the prana conduit
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreatePranaConduit(const FVector& Center);

    // Apply prana effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyPranaEffects(class AEchoesCharacter* Target);

    // Calculate prana flow
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculatePranaFlow(AEchoesCharacter* Target);

    // Timer handle for conduit duration
    FTimerHandle ConduitTimer;
}; 