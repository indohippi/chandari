#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "GoldenAge.generated.h"

UENUM(BlueprintType)
enum class EAgeType : uint8
{
    Harmony UMETA(DisplayName = "Harmony"),
    Prosperity UMETA(DisplayName = "Prosperity"),
    Perfection UMETA(DisplayName = "Perfection")
};

UCLASS()
class ECHOESOFCREATION_API AGoldenAge : public AActor
{
    GENERATED_BODY()

public:
    AGoldenAge();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeAge(EAgeType AgeType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateAge();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateAge();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SetAgeRadius(float Radius);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    EAgeType CurrentAgeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    float AgePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    float AgeDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    float AgeRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Age Properties")
    TArray<AActor*> AffectedActors;

    UFUNCTION(BlueprintImplementableEvent, Category = "Age Effects")
    void OnAgeActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Age Effects")
    void OnAgeDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Age Effects")
    void OnActorAffected(AActor* Actor, float HarmonyStrength);

private:
    void ApplyAgeEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateAgeState(float DeltaTime);
    void CheckAffectedActors();
    float CalculateHarmonyStrength(AActor* Target);
    void FindActorsInRadius();
}; 