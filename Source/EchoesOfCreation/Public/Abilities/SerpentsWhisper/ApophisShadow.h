#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "ApophisShadow.generated.h"

UENUM(BlueprintType)
enum class EShadowType : uint8
{
    Deception UMETA(DisplayName = "Deception"),
    Concealment UMETA(DisplayName = "Concealment"),
    Chaos UMETA(DisplayName = "Chaos")
};

UCLASS()
class ECHOESOFCREATION_API AApophisShadow : public AActor
{
    GENERATED_BODY()

public:
    AApophisShadow();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeShadow(EShadowType ShadowType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateShadow();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateShadow();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ExpandShadow(float ExpansionRate);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    EShadowType CurrentShadowType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    float ShadowPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    float ShadowDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    float ShadowRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    float StealthFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Properties")
    float OriginalWalkSpeed;

    UFUNCTION(BlueprintImplementableEvent, Category = "Shadow Effects")
    void OnShadowActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Shadow Effects")
    void OnShadowDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Shadow Effects")
    void OnShadowExpanded(float NewRadius);

    UFUNCTION(BlueprintImplementableEvent, Category = "Shadow Effects")
    void OnIllusionCreated(const FVector& Location);

private:
    void ApplyShadowEffects();
    void CreateDeceptionEffects();
    void ApplyConcealmentEffects(class ACharacter* OwnerCharacter);
    void ApplyChaosEffects();
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateShadowState(float DeltaTime);
    bool CheckTargetValidity(AActor* Target);
}; 