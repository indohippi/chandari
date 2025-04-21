#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "YinYangShift.generated.h"

UENUM(BlueprintType)
enum class EBalanceType : uint8
{
    YinDominance UMETA(DisplayName = "Yin Dominance"),
    YangDominance UMETA(DisplayName = "Yang Dominance"),
    PerfectBalance UMETA(DisplayName = "Perfect Balance")
};

UCLASS()
class ECHOESOFCREATION_API AYinYangShift : public AActor
{
    GENERATED_BODY()

public:
    AYinYangShift();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeBalance(EBalanceType BalanceType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateBalance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateBalance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ShiftBalance(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    EBalanceType CurrentBalanceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    float BalancePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    float BalanceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    float BalanceRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Balance Effects")
    void OnBalanceActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Balance Effects")
    void OnBalanceDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Balance Effects")
    void OnBalanceShifted(AActor* Target, float BalanceValue);

private:
    void ApplyBalanceEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateBalanceState(float DeltaTime);
    void CheckTargetValidity();
    float CalculateBalanceValue(AActor* Target);
}; 