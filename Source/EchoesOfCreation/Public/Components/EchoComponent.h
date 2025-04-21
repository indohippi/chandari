#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Echoes/EchoTypes.h"
#include "EchoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoDetected, const FEchoData&, EchoData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoEffectApplied, const FEchoEffect&, EchoEffect);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOESOFCREATION_API UEchoComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UEchoComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Detect echoes in range
    UFUNCTION(BlueprintCallable, Category = "Echo")
    void DetectEchoes(float Range = 1000.0f);

    // Interact with a specific echo
    UFUNCTION(BlueprintCallable, Category = "Echo")
    void InteractWithEcho(const FEchoData& EchoData);

    // Get current echo effects
    UFUNCTION(BlueprintCallable, Category = "Echo")
    FEchoEffect GetCurrentEchoEffect() const { return CurrentEchoEffect; }

    // Echo detection delegates
    UPROPERTY(BlueprintAssignable, Category = "Echo")
    FOnEchoDetected OnEchoDetected;

    UPROPERTY(BlueprintAssignable, Category = "Echo")
    FOnEchoEffectApplied OnEchoEffectApplied;

protected:
    // Current echo effect
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Echo")
    FEchoEffect CurrentEchoEffect;

    // Echo detection range
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float EchoDetectionRange;

    // Echo interaction cooldown
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float EchoInteractionCooldown;

    // Last interaction time
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Echo")
    float LastInteractionTime;

    // Handle echo effects
    void HandleEchoEffect(const FEchoEffect& Effect);

    // Timer handle for echo detection
    FTimerHandle EchoDetectionTimer;
}; 