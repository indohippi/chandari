#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "DragonsBreath.generated.h"

UENUM(BlueprintType)
enum class EBreathType : uint8
{
    CelestialFlame UMETA(DisplayName = "Celestial Flame"),
    VoidFrost UMETA(DisplayName = "Void Frost"),
    StormWind UMETA(DisplayName = "Storm Wind"),
    EarthForce UMETA(DisplayName = "Earth Force")
};

UCLASS()
class ECHOESOFCREATION_API ADragonsBreath : public AActor
{
    GENERATED_BODY()

public:
    ADragonsBreath();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeBreath(EBreathType BreathType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateBreath();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateBreath();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void TargetBreath(FVector TargetLocation);

protected:
    // Visual Effects Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual Effects")
    UParticleSystemComponent* BreathParticleSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraComponent* BreathNiagaraSystem;

    // Visual Effects Assets
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UParticleSystem* CelestialFlameEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UParticleSystem* VoidFrostEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UParticleSystem* StormWindEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UParticleSystem* EarthForceEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* BreathTrailEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual Effects")
    UNiagaraSystem* BreathImpactEffect;

    // Sound Effects
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound Effects")
    USoundCue* BreathSound;

    // Existing Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    EBreathType CurrentBreathType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float ElementalPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    FVector CurrentTargetLocation;

    // Combo System Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo System")
    float ComboWindow = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo System")
    int32 CurrentComboCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo System")
    float ComboMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo System")
    TMap<EBreathType, EBreathType> ComboTransitions;

    // Environmental Interaction Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Effects")
    float TerrainModificationRadius = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environmental Effects")
    float TerrainModificationStrength = 0.5f;

    // Blueprint Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathTargeted(FVector TargetLocation);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combo System")
    void OnComboProgress(int32 ComboCount, float ComboMultiplier);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combo System")
    void OnComboReset();

private:
    void LoadVisualEffectAssets();
    void ApplyElementalEffects(AActor* Target, float DeltaTime);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateBreathState(float DeltaTime);
    void CheckAffectedActors();
    float CalculateEffectStrength(AActor* Target);
    
    // Combo System Functions
    void UpdateComboSystem(EBreathType NewType);
    void ResetCombo();
    void ApplyComboEffects(AActor* Target, float DeltaTime);
    
    // Environmental Interaction Functions
    void ModifyTerrain(const FVector& Location);
    void CreateEnvironmentalEffects(const FVector& Location);
    void HandleTerrainInteractions(AActor* Target);

    // Timer handles
    FTimerHandle ComboResetTimer;
    FTimerHandle TerrainModificationTimer;
}; 