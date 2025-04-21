#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "ShemHaMephorashSeverance.generated.h"

UENUM(BlueprintType)
enum class ESeveranceType : uint8
{
    NameOfCreation UMETA(DisplayName = "Name of Creation"),
    NameOfDestruction UMETA(DisplayName = "Name of Destruction"),
    NameOfBalance UMETA(DisplayName = "Name of Balance")
};

UCLASS()
class ECHOESOFCREATION_API AShemHaMephorashSeverance : public AActor
{
    GENERATED_BODY()

public:
    AShemHaMephorashSeverance();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeSeverance(ESeveranceType SeveranceType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateSeverance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateSeverance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SeverTarget(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    ESeveranceType CurrentSeveranceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    float SeverancePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    float SeveranceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    float SeveranceRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Severance Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Severance Effects")
    void OnSeveranceActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Severance Effects")
    void OnSeveranceDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Severance Effects")
    void OnTargetSevered(AActor* Target, float SeveranceStrength);

private:
    void ApplySeveranceEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateSeveranceState(float DeltaTime);
    bool CheckTargetValidity(AActor* Target);
    float CalculateSeveranceStrength(AActor* Target);
    void ModifyReality(AActor* Target, float DeltaTime);
}; 