#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FrostTerrainEffect.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOESOFCREATION_API UFrostTerrainEffect : public UActorComponent
{
    GENERATED_BODY()

public:
    UFrostTerrainEffect();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Frost Effect")
    void ApplyFrostEffect(const FVector& Location, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Frost Effect")
    void Cleanup();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Frost Effect")
    float FrostRadius;

    UPROPERTY(EditDefaultsOnly, Category = "Frost Effect")
    float FrostDuration;

    UPROPERTY(EditDefaultsOnly, Category = "Frost Effect")
    float FrostIntensity;

    UPROPERTY(EditDefaultsOnly, Category = "Frost Effect")
    float FrostDecayRate;

private:
    void LoadEffectAssets();
    void DecayFrostEffect();

    UPROPERTY()
    UNiagaraSystem* FrostTerrainEffect;

    UPROPERTY()
    UMaterialInterface* FrostDecalMaterial;

    UPROPERTY()
    TArray<UNiagaraComponent*> ActiveEffects;

    UPROPERTY()
    TArray<UDecalComponent*> ActiveDecals;

    UPROPERTY()
    TMap<UPhysicalMaterial*, float> OriginalFriction;

    FTimerHandle FrostDecayTimer;
}; 