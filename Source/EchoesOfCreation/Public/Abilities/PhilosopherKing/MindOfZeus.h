#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResonanceTypes.h"
#include "EchoTypes.h"
#include "MindOfZeus.generated.h"

UENUM(BlueprintType)
enum class EKnowledgeType : uint8
{
    DivineWisdom UMETA(DisplayName = "Divine Wisdom"),
    CosmicInsight UMETA(DisplayName = "Cosmic Insight"),
    EternalTruth UMETA(DisplayName = "Eternal Truth")
};

UCLASS()
class ECHOESOFCREATION_API AMindOfZeus : public AActor
{
    GENERATED_BODY()

public:
    AMindOfZeus();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeKnowledge(EKnowledgeType KnowledgeType, float BasePower, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateKnowledge();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateKnowledge();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void TargetKnowledge(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    EKnowledgeType CurrentKnowledgeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    float KnowledgePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    float KnowledgeDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    float KnowledgeRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    float ResonanceGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    TMap<EResonanceType, float> ResonanceModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    TMap<EEchoType, float> EchoInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knowledge Properties")
    AActor* CurrentTarget;

    UFUNCTION(BlueprintImplementableEvent, Category = "Knowledge Effects")
    void OnKnowledgeActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Knowledge Effects")
    void OnKnowledgeDeactivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Knowledge Effects")
    void OnTargetEnlightened(AActor* Target, float EnlightenmentStrength);

private:
    void ApplyKnowledgeEffects(AActor* Target);
    void GenerateResonance();
    void HandleEchoInteractions();
    void UpdateKnowledgeState(float DeltaTime);
    void CheckTargetValidity();
    float CalculateEnlightenmentStrength(AActor* Target);
}; 