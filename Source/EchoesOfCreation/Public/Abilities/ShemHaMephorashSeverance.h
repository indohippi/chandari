#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "ShemHaMephorashSeverance.generated.h"

UENUM(BlueprintType)
enum class EShemType : uint8
{
    Keter,      // Crown - Ultimate authority
    Chokmah,    // Wisdom - Divine insight
    Binah,      // Understanding - Divine knowledge
    Chesed,     // Mercy - Divine love
    Geburah,    // Strength - Divine judgment
    Tiferet,    // Beauty - Divine harmony
    Netzach,    // Victory - Divine endurance
    Hod,        // Glory - Divine splendor
    Yesod,      // Foundation - Divine connection
    Malkuth     // Kingdom - Divine manifestation
};

UCLASS()
class ECHOESOFCREATION_API AShemHaMephorashSeverance : public ABaseAbility
{
    GENERATED_BODY()

public:
    AShemHaMephorashSeverance();

    // Override base ability functions
    virtual bool ActivateAbility(const FVector& TargetLocation) override;

    // Select a specific Sefirah
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SelectSefirah(EShemType Sefirah);

protected:
    // Ability-specific properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float SeveranceRadius = 400.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float SeveranceDuration = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    float DivineIntensity = 0.6f;

    // Currently selected Sefirah
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    EShemType SelectedSefirah;

    // Override base ability functions
    virtual void ApplyEffects(const FVector& TargetLocation) override;

    // Create the severance field
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void CreateSeveranceField(const FVector& Center);

    // Apply severance effects to targets
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ApplySeveranceEffects(class AEchoesCharacter* Target);

    // Timer handle for severance duration
    FTimerHandle SeveranceTimer;
}; 