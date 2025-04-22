#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "AvatarsStrike.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInterface;
class UPostProcessComponent;

UENUM(BlueprintType)
enum class EStrikeType : uint8
{
    CosmicBlade UMETA(DisplayName = "Cosmic Blade"),
    DivineHammer UMETA(DisplayName = "Divine Hammer"),
    EternalSpear UMETA(DisplayName = "Eternal Spear")
};

UCLASS()
class ECHOESOFCREATION_API AAvatarsStrike : public AActor
{
    GENERATED_BODY()

public:
    AAvatarsStrike();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Avatar's Strike")
    void SelectStrikeType(EStrikeType Type);

    UFUNCTION(BlueprintCallable, Category = "Avatar's Strike")
    bool ActivateAbility(const FVector& TargetLocation);

protected:
    // Visual Effects
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* StrikeEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* ImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* TrailEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UMaterialInterface* StrikeDecal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UMaterialInterface* PostProcessMaterial;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPostProcessComponent* PostProcessComponent;

    // Properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar's Strike")
    float StrikeRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar's Strike")
    float StrikePower;

    UPROPERTY()
    EStrikeType SelectedType;

    // Resonance and Echo interactions
    UPROPERTY()
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY()
    TMap<EEchoType, float> EchoInteractions;

    // Blueprint events
    UFUNCTION(BlueprintImplementableEvent, Category = "Avatar's Strike")
    void OnStrikeActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Avatar's Strike")
    void OnStrikeImpact(AActor* Target, float ImpactStrength);

private:
    void CreateStrikeEffect(const FVector& TargetLocation);
    void ApplyEffects(const FVector& TargetLocation);
    void ApplyStrikeEffects(AActor* Target);
    float CalculateStrikeStrength(AActor* Target);
}; 