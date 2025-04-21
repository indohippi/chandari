#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "SerpentsEmbrace.generated.h"

UENUM(BlueprintType)
enum class EEmbraceType : uint8
{
    CreationCoil UMETA(DisplayName = "Creation Coil"),
    DestructionFang UMETA(DisplayName = "Destruction Fang"),
    EternalCycle UMETA(DisplayName = "Eternal Cycle")
};

UCLASS()
class ECHOESOFCREATION_API ASerpentsEmbrace : public AActor
{
    GENERATED_BODY()

public:
    ASerpentsEmbrace();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeEmbrace(EEmbraceType EmbraceType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateEmbrace();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateEmbrace();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void TargetEmbrace(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    EEmbraceType CurrentEmbraceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float EmbracePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float EmbraceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float EmbraceRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnEmbraceActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnEmbraceDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnTargetEmbraced(AActor* Target);

private:
    void ApplyEmbraceEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateEmbraceState(float DeltaTime);
    void CheckTargetValidity();
}; 