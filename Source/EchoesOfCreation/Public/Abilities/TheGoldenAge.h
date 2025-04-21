#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "TheGoldenAge.generated.h"

UENUM(BlueprintType)
enum class EGoldenAgeAspect : uint8
{
    Prosperity, // Abundance and growth
    Harmony,    // Peace and balance
    Knowledge,  // Wisdom and understanding
    Justice,    // Fairness and order
    Unity       // Cooperation and strength
};

UCLASS()
class ECHOESOFCREATION_API ATheGoldenAge : public ABaseAbility
{
    GENERATED_BODY()

public:
    ATheGoldenAge();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific aspect of the Golden Age
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectGoldenAgeAspect(EGoldenAgeAspect Aspect);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float GoldenAgeRadius = 500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float GoldenAgeDuration = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float GoldenAgeIntensity = 0.9f;

    // Currently selected aspect
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EGoldenAgeAspect SelectedAspect;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the Golden Age field
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateGoldenAgeField(const FVector& Center);

    // Apply Golden Age effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplyGoldenAgeEffects(class AEchoesCharacter* Target);

    // Timer handle for Golden Age duration
    FTimerHandle GoldenAgeTimer;
}; 