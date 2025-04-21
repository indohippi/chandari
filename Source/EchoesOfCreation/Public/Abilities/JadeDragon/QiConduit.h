#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "QiConduit.generated.h"

UENUM(BlueprintType)
enum class EQiType : uint8
{
    VitalFlow UMETA(DisplayName = "Vital Flow"),
    MeridianSurge UMETA(DisplayName = "Meridian Surge"),
    SpiritChannel UMETA(DisplayName = "Spirit Channel")
};

UCLASS()
class ECHOESOFCREATION_API AQiConduit : public AActor
{
    GENERATED_BODY()

public:
    AQiConduit();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeConduit(EQiType QiType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateConduit();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateConduit();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ChannelQi(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    EQiType CurrentQiType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float QiPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ConduitDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ChannelRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnConduitActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnConduitDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnQiChanneled(AActor* Target, float QiStrength);

private:
    void ApplyQiEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateConduitState(float DeltaTime);
    void CheckTargetValidity();
    float CalculateQiStrength(AActor* Target);
}; 