#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EchoActor.generated.h"

UENUM(BlueprintType)
enum class EEchoType : uint8
{
    Weaver,
    Static,
    Resonance,
    Corrupted
};

UCLASS()
class ECHOESOFCREATION_API AEchoActor : public AActor
{
    GENERATED_BODY()
    
public:    
    AEchoActor();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;

    // Echo Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    EEchoType EchoType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float EchoStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float EchoRadius = 500.0f;

    // Echo Effects
    UFUNCTION(BlueprintImplementableEvent, Category = "Echo")
    void OnEchoManifest();

    UFUNCTION(BlueprintImplementableEvent, Category = "Echo")
    void OnEchoInteract(AActor* InteractingActor);

    UFUNCTION(BlueprintImplementableEvent, Category = "Echo")
    void OnEchoDissipate();

    // Echo Behavior
    UFUNCTION(BlueprintCallable, Category = "Echo")
    void UpdateEchoStrength(float DeltaStrength);

    UFUNCTION(BlueprintCallable, Category = "Echo")
    void TriggerEchoEffect(AActor* TargetActor);

private:
    UPROPERTY(VisibleAnywhere, Category = "Echo")
    class USphereComponent* EchoSphere;

    UPROPERTY(VisibleAnywhere, Category = "Echo")
    class UParticleSystemComponent* EchoParticles;

    void InitializeEcho();
    void UpdateEchoEffects();
}; 