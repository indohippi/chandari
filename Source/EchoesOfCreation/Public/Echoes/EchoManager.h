#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EchoTypes.h"
#include "Resonance/ResonanceTypes.h"
#include "EchoManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoManifested, const FEchoData&, EchoData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoEffectApplied, const FEchoEffect&, EchoEffect);

UCLASS()
class ECHOESOFCREATION_API UEchoManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Echo")
    void ProcessResonance(const FResonanceData& ResonanceData);

    UFUNCTION(BlueprintCallable, Category = "Echo")
    FEchoEffect CalculateEchoEffect(const FEchoData& EchoData) const;

    UFUNCTION(BlueprintCallable, Category = "Echo")
    void RegisterEcho(const FEchoData& EchoData);

    UPROPERTY(BlueprintAssignable, Category = "Echo")
    FOnEchoManifested OnEchoManifested;

    UPROPERTY(BlueprintAssignable, Category = "Echo")
    FOnEchoEffectApplied OnEchoEffectApplied;

protected:
    UPROPERTY()
    TArray<FEchoData> ActiveEchoes;

    UPROPERTY()
    TMap<FString, FEchoData> KnownEchoes;

    UFUNCTION()
    void UpdateEchoEffects(float DeltaTime);

    FTimerHandle EchoUpdateTimer;

    void CheckEchoManifestation(const FResonanceData& ResonanceData);
    void ApplyEchoEffects(const FEchoData& EchoData);
}; 