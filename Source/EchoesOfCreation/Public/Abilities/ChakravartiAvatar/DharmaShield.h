#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "DharmaShield.generated.h"

UENUM(BlueprintType)
enum class EDharmaType : uint8
{
    Protection UMETA(DisplayName = "Protection"),
    Justice UMETA(DisplayName = "Justice"),
    Balance UMETA(DisplayName = "Balance")
};

UCLASS()
class ECHOESOFCREATION_API ADharmaShield : public AActor
{
    GENERATED_BODY()

public:
    ADharmaShield();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeShield(EDharmaType DharmaType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateShield();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateShield();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SetShieldRadius(float Radius);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    EDharmaType CurrentDharmaType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    float ShieldPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    float ShieldDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    float ShieldRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Properties")
    TArray<AActor*> ProtectedActors;

    UFUNCTION(BlueprintImplementableEvent, Category = "Shield Effects")
    void OnShieldActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Shield Effects")
    void OnShieldDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Shield Effects")
    void OnActorProtected(AActor* Actor, float ProtectionStrength);

private:
    void ApplyShieldEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateShieldState(float DeltaTime);
    void CheckProtectedActors();
    float CalculateProtectionStrength(AActor* Target);
    void FindActorsInRadius();
}; 