#include "ResonatorCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AResonatorCharacter::AResonatorCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Create follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Create resonance component
    ResonanceComponent = CreateDefaultSubobject<UResonanceComponent>(TEXT("ResonanceComponent"));

    // Set default values
    bIsSprinting = false;
    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AResonatorCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AResonatorCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSprinting)
    {
        UpdateResonanceFromMovement(DeltaTime);
    }
}

void AResonatorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement bindings
    PlayerInputComponent->BindAxis("MoveForward", this, &AResonatorCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AResonatorCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AResonatorCharacter::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AResonatorCharacter::Turn);

    // Action bindings
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AResonatorCharacter::SprintPressed);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AResonatorCharacter::SprintReleased);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AResonatorCharacter::InteractPressed);
}

void AResonatorCharacter::GenerateResonance(EResonanceType Type, float Intensity)
{
    if (ResonanceComponent)
    {
        ResonanceComponent->AddResonance(Type, Intensity);
    }
}

void AResonatorCharacter::ConsumeResonance(EResonanceType Type, float Amount)
{
    if (ResonanceComponent)
    {
        ResonanceComponent->RemoveResonance(Type, Amount);
    }
}

void AResonatorCharacter::ActivateResonanceShield()
{
    if (CanUseAbility(EResonanceType::Faith, ShieldResonanceCost))
    {
        ConsumeResonance(EResonanceType::Faith, ShieldResonanceCost);
        // TODO: Implement shield activation logic
    }
}

void AResonatorCharacter::ActivateEchoSight()
{
    if (CanUseAbility(EResonanceType::Curiosity, EchoSightResonanceCost))
    {
        ConsumeResonance(EResonanceType::Curiosity, EchoSightResonanceCost);
        // TODO: Implement echo sight activation logic
    }
}

void AResonatorCharacter::ActivateResonanceManipulation()
{
    if (CanUseAbility(EResonanceType::Doubt, ManipulationResonanceCost))
    {
        ConsumeResonance(EResonanceType::Doubt, ManipulationResonanceCost);
        // TODO: Implement resonance manipulation logic
    }
}

void AResonatorCharacter::StartSprint()
{
    if (!bIsSprinting)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
    }
}

void AResonatorCharacter::StopSprint()
{
    if (bIsSprinting)
    {
        bIsSprinting = false;
        GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
    }
}

void AResonatorCharacter::Interact()
{
    // TODO: Implement interaction logic
}

void AResonatorCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AResonatorCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AResonatorCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AResonatorCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AResonatorCharacter::SprintPressed()
{
    StartSprint();
}

void AResonatorCharacter::SprintReleased()
{
    StopSprint();
}

void AResonatorCharacter::InteractPressed()
{
    Interact();
}

void AResonatorCharacter::UpdateResonanceFromMovement(float DeltaTime)
{
    if (ResonanceComponent)
    {
        ResonanceComponent->AddResonance(EResonanceType::Curiosity, SprintResonanceCost * DeltaTime);
    }
}

bool AResonatorCharacter::CanUseAbility(EResonanceType Type, float Cost)
{
    if (ResonanceComponent)
    {
        return ResonanceComponent->GetResonance(Type) >= Cost;
    }
    return false;
} 