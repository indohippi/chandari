#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "QliphothSeal.generated.h"

UENUM(BlueprintType)
enum class EQliphothType : uint8
{
    Thaumiel,   // Duality and corruption
    Ghagiel,    // Chaos and disruption
    Sathariel,  // Concealment and deception
    Gamchicoth, // Destruction and decay
    Golachab    // Burning and purification
};

UCLASS()
class ECHOESOFCREATION_API AQliphothSeal : public ABaseAbility
{
    GENERATED_BODY()

public:
    AQliphothSeal();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific Qliphoth type
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectQliphothType(EQliphothType Type);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float SealRadius = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float SealDuration = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float CorruptionIntensity = 0.5f;

    // Currently selected Qliphoth type
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EQliphothType SelectedType;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the seal
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateSeal(const FVector& Center);

    // Apply seal effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplySealEffects(class AEchoesCharacter* Target);

    // Timer handle for seal duration
    FTimerHandle SealTimer;
}; 