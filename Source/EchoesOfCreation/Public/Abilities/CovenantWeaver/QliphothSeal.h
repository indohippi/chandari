#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "QliphothSeal.generated.h"

UENUM(BlueprintType)
enum class ESealType : uint8
{
    Containment UMETA(DisplayName = "Containment"),
    Purification UMETA(DisplayName = "Purification"),
    Transformation UMETA(DisplayName = "Transformation")
};

UCLASS()
class ECHOESOFCREATION_API AQliphothSeal : public AActor
{
    GENERATED_BODY()

public:
    AQliphothSeal();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeSeal(ESealType SealType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateSeal();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateSeal();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void PlaceSeal(FVector Location);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    ESealType CurrentSealType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    float SealPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    float SealDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    float SealRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    FVector SealLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seal Properties")
    TArray<AActor*> SealedActors;

    UFUNCTION(BlueprintImplementableEvent, Category = "Seal Effects")
    void OnSealActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Seal Effects")
    void OnSealDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Seal Effects")
    void OnActorSealed(AActor* Actor, float SealStrength);

private:
    void ApplySealEffects(AActor* Actor);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateSealState(float DeltaTime);
    void CheckSealedActors();
    float CalculateSealStrength(AActor* Actor);
    void ProcessSealedActors(float DeltaTime);
}; 