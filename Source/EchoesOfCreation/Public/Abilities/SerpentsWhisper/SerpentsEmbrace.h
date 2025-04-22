#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "SerpentsEmbrace.generated.h"

UENUM(BlueprintType)
enum class EEmbraceType : uint8
{
    Constriction UMETA(DisplayName = "Constriction"),
    Envelopment UMETA(DisplayName = "Envelopment"),
    Absorption UMETA(DisplayName = "Absorption")
};

USTRUCT()
struct FConstrictedTargetData
{
    GENERATED_BODY()

    float OriginalMaxSpeed;
    float ConstrictionTime;
    float ConstrictionStrength;
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

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    EEmbraceType CurrentEmbraceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float ConstrictionPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float EmbraceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float EmbraceRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    int32 MaxTargets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embrace Properties")
    TMap<AActor*, FConstrictedTargetData> ConstrictedTargets;

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnEmbraceActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnEmbraceDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnTargetConstricted(AActor* Target, float ConstrictionStrength);

    UFUNCTION(BlueprintImplementableEvent, Category = "Embrace Effects")
    void OnTargetReleased(AActor* Target);

private:
    void FindTargetsInRange();
    bool CanConstrictTarget(AActor* Target);
    void ConstrictTarget(AActor* Target);
    void ReleaseTarget(AActor* Target);
    void ApplyConstrictionEffects(float DeltaTime);
    void ApplyConstrictionDamage(class ACharacter* Target, float Strength, float DeltaTime);
    void ApplyEnvelopmentEffects(class ACharacter* Target, float Strength, float DeltaTime);
    void ApplyAbsorptionEffects(class ACharacter* Target, float Strength, float DeltaTime);
    float CalculateConstrictionStrength(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateEmbraceState(float DeltaTime);
}; 