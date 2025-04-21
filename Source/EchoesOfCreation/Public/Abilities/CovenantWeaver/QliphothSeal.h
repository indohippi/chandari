#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "QliphothSeal.generated.h"

UENUM(BlueprintType)
enum class ESealType : uint8
{
    Thaumiel UMETA(DisplayName = "Thaumiel"),
    Gamaliel UMETA(DisplayName = "Gamaliel"),
    Samael UMETA(DisplayName = "Samael")
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

    UFUNCTION(BlueprintImplementableEvent, Category = "Seal Effects")
    void OnSealActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Seal Effects")
    void OnSealDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Seal Effects")
    void OnSealPlaced(FVector Location, float SealStrength);

private:
    void ApplySealEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateSealState(float DeltaTime);
    void CheckAffectedActors();
    float CalculateSealStrength(AActor* Target);
}; 