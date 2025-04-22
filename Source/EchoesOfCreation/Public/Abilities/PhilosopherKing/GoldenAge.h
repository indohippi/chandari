#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "GoldenAge.generated.h"

class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UPostProcessComponent;
class ACharacter;

UENUM(BlueprintType)
enum class EGoldenAgeType : uint8
{
    Enlightenment UMETA(DisplayName = "Enlightenment"),
    Harmony      UMETA(DisplayName = "Harmony"),
    Prosperity   UMETA(DisplayName = "Prosperity"),
    Transcendence UMETA(DisplayName = "Transcendence")
};

UCLASS()
class ECHOESOFCREATION_API AGoldenAge : public AActor
{
    GENERATED_BODY()

public:
    AGoldenAge();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Golden Age")
    void InitializeGoldenAge(EGoldenAgeType AgeType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Golden Age")
    void ActivateGoldenAge();

    UFUNCTION(BlueprintCallable, Category = "Golden Age")
    void DeactivateGoldenAge();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* EnlightenmentSphere;

    // Visual Effects
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* AuraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* SymbolsEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* GroundEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* AffectedCharacterEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UMaterialInterface* PostProcessMaterial;

    // Effect Components
    UPROPERTY()
    UNiagaraComponent* AuraComponent;

    UPROPERTY()
    UNiagaraComponent* SymbolsComponent;

    UPROPERTY()
    UNiagaraComponent* GroundComponent;

    UPROPERTY()
    UPostProcessComponent* PostProcessComponent;

    UPROPERTY()
    UMaterialInstanceDynamic* PostProcessInstance;

    // Map to track affected character effects
    UPROPERTY()
    TMap<ACharacter*, UNiagaraComponent*> AffectedCharacterEffects;

    // Properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Golden Age")
    float EnlightenmentRange;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Golden Age")
    float WisdomPower;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Golden Age")
    float HarmonyFactor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Golden Age")
    float ResonanceGenerationRate;

    UPROPERTY()
    float AgeDuration;

    UPROPERTY()
    float OriginalTimeScale;

    UPROPERTY()
    bool bIsActive;

    UPROPERTY()
    EGoldenAgeType CurrentAgeType;

    // Resonance and Echo interactions
    UPROPERTY()
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY()
    TMap<EEchoType, float> EchoInteractions;

    // Blueprint events
    UFUNCTION(BlueprintImplementableEvent, Category = "Golden Age")
    void OnGoldenAgeActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Golden Age")
    void OnGoldenAgeDeactivated();

private:
    void UpdateGoldenAgeState(float DeltaTime);
    void ApplyEnlightenmentEffects(float DeltaTime);
    void ApplyAgeEffects(ACharacter* Target, float Strength, float DeltaTime);
    float CalculateEnlightenmentStrength(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();

    // Visual effect functions
    void SpawnManifestationEffects();
    void UpdateManifestationEffects(float DeltaTime);
    void CleanupManifestationEffects();
}; 