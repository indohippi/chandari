#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "PhilosophersStoneTransmutation.generated.h"

UENUM(BlueprintType)
enum class ETransmutationType : uint8
{
    PrimaMateria UMETA(DisplayName = "Prima Materia"),
    MagnumOpus UMETA(DisplayName = "Magnum Opus"),
    PhilosophersStone UMETA(DisplayName = "Philosopher's Stone")
};

UCLASS()
class ECHOESOFCREATION_API APhilosophersStoneTransmutation : public AActor
{
    GENERATED_BODY()

public:
    APhilosophersStoneTransmutation();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeTransmutation(ETransmutationType TransmutationType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateTransmutation();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateTransmutation();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void TargetTransmutation(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    ETransmutationType CurrentTransmutationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    float TransmutationPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    float TransmutationDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    float TransmutationRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transmutation Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Transmutation Effects")
    void OnTransmutationActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Transmutation Effects")
    void OnTransmutationDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Transmutation Effects")
    void OnTargetTransmuted(AActor* Target, float TransmutationStrength);

private:
    void ApplyTransmutationEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateTransmutationState(float DeltaTime);
    void CheckTargetValidity();
    float CalculateTransmutationStrength(AActor* Target);
}; 