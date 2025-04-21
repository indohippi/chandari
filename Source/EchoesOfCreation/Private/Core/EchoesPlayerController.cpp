#include "Core/EchoesPlayerController.h"
#include "Characters/EchoesCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

AEchoesPlayerController::AEchoesPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AEchoesPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void AEchoesPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // Movement
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEchoesPlayerController::OnMove);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEchoesPlayerController::OnLook);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEchoesPlayerController::OnJump);

        // Abilities
        EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Started, this, &AEchoesPlayerController::OnAbility1);
        EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Started, this, &AEchoesPlayerController::OnAbility2);
        EnhancedInputComponent->BindAction(Ability3Action, ETriggerEvent::Started, this, &AEchoesPlayerController::OnAbility3);

        // Interaction
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AEchoesPlayerController::OnInteract);
    }
}

void AEchoesPlayerController::OnMove(const FInputActionValue& Value)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        const FVector2D MovementVector = Value.Get<FVector2D>();
        UpdateCharacterMovement(MovementVector);
    }
}

void AEchoesPlayerController::OnLook(const FInputActionValue& Value)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        const FVector2D LookAxisVector = Value.Get<FVector2D>();
        UpdateCharacterRotation(LookAxisVector);
    }
}

void AEchoesPlayerController::OnJump(const FInputActionValue& Value)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        Character->Jump();
    }
}

void AEchoesPlayerController::OnAbility1(const FInputActionValue& Value)
{
    ActivateAbility(0);
}

void AEchoesPlayerController::OnAbility2(const FInputActionValue& Value)
{
    ActivateAbility(1);
}

void AEchoesPlayerController::OnAbility3(const FInputActionValue& Value)
{
    ActivateAbility(2);
}

void AEchoesPlayerController::OnInteract(const FInputActionValue& Value)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        // Handle interaction with echoes or other game elements
    }
}

AEchoesPlayerState* AEchoesPlayerController::GetEchoesPlayerState() const
{
    return Cast<AEchoesPlayerState>(PlayerState);
}

AEchoesCharacter* AEchoesPlayerController::GetEchoesCharacter() const
{
    return Cast<AEchoesCharacter>(GetPawn());
}

void AEchoesPlayerController::ActivateAbility(int32 AbilityIndex)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        Character->ActivateAbility(AbilityIndex);
    }
}

void AEchoesPlayerController::DeactivateAbility(int32 AbilityIndex)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        Character->DeactivateAbility(AbilityIndex);
    }
}

void AEchoesPlayerController::UpdateCharacterMovement(const FInputActionValue& Value)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        // Find out which way is forward
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // Get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Add movement 
        Character->AddMovementInput(ForwardDirection, Value.Y);
        Character->AddMovementInput(RightDirection, Value.X);
    }
}

void AEchoesPlayerController::UpdateCharacterRotation(const FInputActionValue& Value)
{
    if (AEchoesCharacter* Character = GetEchoesCharacter())
    {
        // Add yaw and pitch input to controller
        Character->AddControllerYawInput(Value.X);
        Character->AddControllerPitchInput(Value.Y);
    }
} 