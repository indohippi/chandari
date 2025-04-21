#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "DivineNameResonance.generated.h"

UENUM(BlueprintType)
enum class EResonanceType : uint8
{
    Tetragrammaton UMETA(DisplayName = "Tetragrammaton"),
    Elohim UMETA(DisplayName = "Elohim"),
    ShemHaMephorash UMETA(DisplayName = "Shem HaMephorash")
};

UCLASS()
class ECHOESOFCREATION_API ADivineNameResonance : public AActor
{
    GENERATED_BODY()

public:
    ADivineNameResonance();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeResonance(EResonanceType NameType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateResonance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateResonance();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ChannelResonance(FVector Location);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    EResonanceType CurrentNameType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonancePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonanceDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonanceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Properties")
    FVector ResonanceLocation;

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance Effects")
    void OnResonanceActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance Effects")
    void OnResonanceDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Resonance Effects")
    void OnResonanceChanneled(FVector Location, float ResonanceStrength);

private:
    void ApplyResonanceEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateResonanceState(float DeltaTime);
    void CheckAffectedActors();
    float CalculateResonanceStrength(AActor* Target);
}; 