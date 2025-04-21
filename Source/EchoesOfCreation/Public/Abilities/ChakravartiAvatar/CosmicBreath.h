#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "CosmicBreath.generated.h"

UENUM(BlueprintType)
enum class ECosmicBreathType : uint8
{
    SolarFlare UMETA(DisplayName = "Solar Flare"),
    LunarTide UMETA(DisplayName = "Lunar Tide"),
    StellarWind UMETA(DisplayName = "Stellar Wind")
};

UCLASS()
class ECHOESOFCREATION_API ACosmicBreath : public AActor
{
    GENERATED_BODY()

public:
    ACosmicBreath();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeBreath(ECosmicBreathType BreathType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateBreath();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateBreath();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SetBreathCone(float Angle, float Range);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    ECosmicBreathType CurrentBreathType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathConeAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float BreathRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breath Properties")
    TArray<AActor*> AffectedActors;

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnBreathDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Breath Effects")
    void OnActorAffected(AActor* Actor, float EffectStrength);

private:
    void ApplyBreathEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateBreathState(float DeltaTime);
    void CheckAffectedActors();
    float CalculateEffectStrength(AActor* Target);
    void FindActorsInCone();
    bool IsInBreathCone(AActor* Target);
}; 