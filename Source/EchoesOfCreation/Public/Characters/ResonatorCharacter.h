#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ResonanceComponent.h"
#include "ResonatorCharacter.generated.h"

UCLASS()
class ECHOESOFCREATION_API AResonatorCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AResonatorCharacter();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Resonance Management
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void GenerateResonance(EResonanceType Type, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void ConsumeResonance(EResonanceType Type, float Amount);

    // Resonance Abilities
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateResonanceShield();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateEchoSight();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateResonanceManipulation();

    // Movement and Interaction
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprint();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact();

protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UResonanceComponent* ResonanceComponent;

    // Movement Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeedMultiplier = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintResonanceCost = 0.1f;

    // Ability Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    float ShieldResonanceCost = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    float EchoSightResonanceCost = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    float ManipulationResonanceCost = 25.0f;

    // Input Functions
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);
    void SprintPressed();
    void SprintReleased();
    void InteractPressed();

private:
    bool bIsSprinting;
    float DefaultWalkSpeed;

    void UpdateResonanceFromMovement(float DeltaTime);
    bool CanUseAbility(EResonanceType Type, float Cost);
}; 