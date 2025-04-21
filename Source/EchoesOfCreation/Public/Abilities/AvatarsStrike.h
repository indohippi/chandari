#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "AvatarsStrike.generated.h"

UENUM(BlueprintType)
enum class EStrikeType : uint8
{
    Divine,     // Pure divine energy
    Cosmic,     // Cosmic force
    Karmic,     // Karmic retribution
    Dharma,     // Righteous judgment
    Avatar      // Ultimate avatar power
};

UCLASS()
class ECHOESOFCREATION_API AAvatarsStrike : public ABaseAbility
{
    GENERATED_BODY()

public:
    AAvatarsStrike();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific strike type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectStrikeType(EStrikeType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float StrikeRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float StrikeDamage = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float DivinePower = 1.0f;

    // Currently selected strike type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EStrikeType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the strike effect
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateStrikeEffect(const FVector& Center);

    // Apply strike effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyStrikeEffects(class AEchoesCharacter* Target);

    // Calculate the strike damage
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float CalculateStrikeDamage(AEchoesCharacter* Target);
}; 