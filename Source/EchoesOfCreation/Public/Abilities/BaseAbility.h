#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Echoes/EchoTypes.h"
#include "Resonance/ResonanceTypes.h"
#include "BaseAbility.generated.h"

UCLASS()
class ECHOESOFCREATION_API ABaseAbility : public AActor
{
    GENERATED_BODY()

public:
    ABaseAbility();

    // Initialize the ability
    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual void InitializeAbility(class AEchoesCharacter* OwnerCharacter);

    // Activate the ability
    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual bool ActivateAbility(const FVector& TargetLocation);

    // Get ability properties
    UFUNCTION(BlueprintCallable, Category = "Ability")
    FString GetAbilityName() const { return AbilityName; }

    UFUNCTION(BlueprintCallable, Category = "Ability")
    float GetResonanceCost() const { return ResonanceCost; }

    UFUNCTION(BlueprintCallable, Category = "Ability")
    float GetCooldown() const { return Cooldown; }

    UFUNCTION(BlueprintCallable, Category = "Ability")
    float GetRange() const { return Range; }

protected:
    // Ability properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FString AbilityName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float ResonanceCost = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float Cooldown = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float Range = 500.0f;

    // Owner character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    class AEchoesCharacter* OwnerCharacter;

    // Last activation time
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    float LastActivationTime;

    // Check if ability can be activated
    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual bool CanActivate() const;

    // Apply ability effects
    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual void ApplyEffects(const FVector& TargetLocation);

    // Generate resonance from ability use
    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual void GenerateResonance(EResonanceType Type, float Intensity);
}; 