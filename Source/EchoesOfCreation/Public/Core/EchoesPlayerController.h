#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EchoesPlayerController.generated.h"

class AEchoesCharacter;
class AEchoesPlayerState;

UCLASS()
class ECHOESOFCREATION_API AEchoesPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AEchoesPlayerController();

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // Input Actions
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* Ability1Action;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* Ability2Action;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* Ability3Action;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* InteractAction;

protected:
    // Input Handlers
    UFUNCTION()
    void OnMove(const FInputActionValue& Value);

    UFUNCTION()
    void OnLook(const FInputActionValue& Value);

    UFUNCTION()
    void OnJump(const FInputActionValue& Value);

    UFUNCTION()
    void OnAbility1(const FInputActionValue& Value);

    UFUNCTION()
    void OnAbility2(const FInputActionValue& Value);

    UFUNCTION()
    void OnAbility3(const FInputActionValue& Value);

    UFUNCTION()
    void OnInteract(const FInputActionValue& Value);

    // Player State
    UFUNCTION(BlueprintCallable, Category = "Player State")
    AEchoesPlayerState* GetEchoesPlayerState() const;

    // Character Management
    UFUNCTION(BlueprintCallable, Category = "Character")
    AEchoesCharacter* GetEchoesCharacter() const;

    // Ability Management
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void ActivateAbility(int32 AbilityIndex);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void DeactivateAbility(int32 AbilityIndex);

private:
    void SetupEnhancedInput();
    void UpdateCharacterMovement(const FInputActionValue& Value);
    void UpdateCharacterRotation(const FInputActionValue& Value);
}; 