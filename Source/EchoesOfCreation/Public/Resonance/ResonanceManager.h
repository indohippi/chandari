#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ResonanceTypes.h"
#include "ResonanceManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResonanceChanged, const FResonanceData&, ResonanceData);

UCLASS()
class ECHOESOFCREATION_API UResonanceManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void GenerateResonance(const FResonanceData& ResonanceData);

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    FResonanceEffect CalculateResonanceEffect(const FResonanceData& ResonanceData) const;

    UPROPERTY(BlueprintAssignable, Category = "Resonance")
    FOnResonanceChanged OnResonanceChanged;

protected:
    UPROPERTY()
    TArray<FResonanceData> ActiveResonances;

    UFUNCTION()
    void UpdateResonanceEffects(float DeltaTime);

    FTimerHandle ResonanceUpdateTimer;
}; 