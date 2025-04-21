#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "ScalesOfMaAt.generated.h"

UENUM(BlueprintType)
enum class EBalanceType : uint8
{
    Justice,    // Fair judgment and retribution
    Truth,      // Reveal hidden knowledge
    Order,      // Restore cosmic balance
    Harmony,    // Balance opposing forces
    Maat        // Perfect cosmic order
};

UCLASS()
class ECHOESOFCREATION_API AScalesOfMaAt : public ABaseAbility
{
    GENERATED_BODY()

public:
    AScalesOfMaAt();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific balance type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectBalanceType(EBalanceType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BalanceRadius = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BalanceDuration = 12.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float BalanceIntensity = 0.8f;

    // Currently selected balance type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EBalanceType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the balance effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateBalanceEffect(const FVector& Center);

    // Apply balance effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyBalanceEffects(class AEchoesCharacter* Target);

    // Calculate balance intensity
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateBalanceIntensity(AEchoesCharacter* Target);

    // Timer handle for balance duration
    FTimerHandle BalanceTimer;
}; 