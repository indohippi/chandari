#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "PranaConduit.generated.h"

UENUM(BlueprintType)
enum class EPranaType : uint8
{
    VitalFlow UMETA(DisplayName = "Vital Flow"),
    EnergySurge UMETA(DisplayName = "Energy Surge"),
    LifeCycle UMETA(DisplayName = "Life Cycle")
};

UCLASS()
class ECHOESOFCREATION_API APranaConduit : public AActor
{
    GENERATED_BODY()

public:
    APranaConduit();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeConduit(EPranaType PranaType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateConduit();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateConduit();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ConnectToTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DisconnectFromTarget();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    EPranaType CurrentPranaType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ConduitPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ConduitDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ConduitRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    AActor* ConnectedTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conduit Properties")
    float PranaTransferRate;

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnConduitActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnConduitDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnTargetConnected(AActor* Target, float ConnectionStrength);

    UFUNCTION(BlueprintImplementableEvent, Category = "Conduit Effects")
    void OnTargetDisconnected(AActor* Target);

private:
    void ApplyConduitEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateConduitState(float DeltaTime);
    bool CheckTargetValidity(AActor* Target);
    float CalculateConnectionStrength(AActor* Target);
    void TransferPrana(AActor* Target, float DeltaTime);
}; 