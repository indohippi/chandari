#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    Echo,
    Object,
    Environment,
    NPC
};

UENUM(BlueprintType)
enum class EInteractionState : uint8
{
    Idle,
    Starting,
    InProgress,
    Completing,
    Failed
};

USTRUCT(BlueprintType)
struct FInteractionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    EInteractionType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FString InteractionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionRange = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bRequiresResonance = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    EResonanceType RequiredResonanceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float RequiredResonanceAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float ResonanceGain = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TArray<EResonanceType> AffectedResonanceTypes;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOESOFCREATION_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UInteractionComponent();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Interaction Management
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void StartInteraction(AActor* TargetActor);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void StopInteraction();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    bool CanInteractWith(AActor* TargetActor) const;

    // Interaction Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnInteractionStarted(AActor* TargetActor, const FInteractionData& InteractionData);

    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnInteractionProgress(AActor* TargetActor, float Progress);

    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnInteractionCompleted(AActor* TargetActor, const FInteractionData& InteractionData);

    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void OnInteractionFailed(AActor* TargetActor, const FString& Reason);

    // Resonance Interaction
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ApplyResonanceToTarget(AActor* TargetActor, EResonanceType Type, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void UpdateResonanceEffects(float DeltaTime);

    // Echo Specific
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ProcessEchoInteraction(AActor* EchoActor);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void HandleEchoResonance(AActor* EchoActor, float ResonanceAmount);

protected:
    UPROPERTY(EditAnywhere, Category = "Interaction")
    float InteractionRange = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float InteractionTime = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float ResonanceDecayRate = 0.1f;

    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    AActor* CurrentInteractionTarget;

    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    bool bIsInteracting;

    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    float CurrentInteractionProgress;

    UPROPERTY(VisibleAnywhere, Category = "Interaction")
    EInteractionState CurrentState;

    void UpdateInteractionProgress(float DeltaTime);
    void CompleteInteraction();
    void CancelInteraction();
    FInteractionData GetInteractionData(AActor* TargetActor) const;
    void ApplyResonanceEffects(const FInteractionData& InteractionData);
    void HandleInteractionStateChange(EInteractionState NewState);
}; 