#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "AvatarsStrike.generated.h"

UENUM(BlueprintType)
enum class EStrikeType : uint8
{
    CosmicBlade UMETA(DisplayName = "Cosmic Blade"),
    DivineHammer UMETA(DisplayName = "Divine Hammer"),
    EternalSpear UMETA(DisplayName = "Eternal Spear")
};

UCLASS()
class ECHOESOFCREATION_API AAvatarsStrike : public AActor
{
    GENERATED_BODY()

public:
    AAvatarsStrike();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeStrike(EStrikeType StrikeType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateStrike();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateStrike();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void StrikeTarget(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    EStrikeType CurrentStrikeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    float StrikePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    float StrikeDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    float StrikeRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Strike Effects")
    void OnStrikeActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Strike Effects")
    void OnStrikeDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Strike Effects")
    void OnTargetStruck(AActor* Target, float StrikeStrength);

private:
    void ApplyStrikeEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateStrikeState(float DeltaTime);
    bool CheckTargetValidity(AActor* Target);
    float CalculateStrikeStrength(AActor* Target);
}; 