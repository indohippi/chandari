#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "ScalesOfMaAt.generated.h"

UENUM(BlueprintType)
enum class EScaleType : uint8
{
    BalanceJudgment UMETA(DisplayName = "Balance Judgment"),
    TruthRevelation UMETA(DisplayName = "Truth Revelation"),
    OrderEnforcement UMETA(DisplayName = "Order Enforcement")
};

UCLASS()
class ECHOESOFCREATION_API AScalesOfMaAt : public AActor
{
    GENERATED_BODY()

public:
    AScalesOfMaAt();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeScales(EScaleType ScaleType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateScales();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateScales();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void JudgeTarget(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    EScaleType CurrentScaleType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    float ScalePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    float ScaleDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    float JudgmentRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale Properties")
    bool bIsActive;

    UFUNCTION(BlueprintImplementableEvent, Category = "Scale Effects")
    void OnScalesActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Scale Effects")
    void OnScalesDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Scale Effects")
    void OnTargetJudged(AActor* Target, float JudgmentValue);

private:
    void ApplyJudgmentEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateScaleState(float DeltaTime);
    float CalculateJudgmentValue(AActor* Target);
}; 