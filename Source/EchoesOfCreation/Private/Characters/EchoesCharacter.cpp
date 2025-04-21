#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

AEchoesCharacter::AEchoesCharacter()
{
    // Set up character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create components
    ResonanceComponent = CreateDefaultSubobject<UResonanceComponent>(TEXT("ResonanceComponent"));
    EchoComponent = CreateDefaultSubobject<UEchoComponent>(TEXT("EchoComponent"));

    // Initialize variables
    LastResonanceTime = 0.0f;
    RemainingActionPoints = ActionPoints;
    CurrentGridPosition = FVector::ZeroVector;
}

void AEchoesCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AEchoesCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AEchoesCharacter::MoveToGridPosition(const FVector& GridPosition)
{
    if (RemainingActionPoints <= 0)
    {
        return false;
    }

    // Calculate distance to target
    float Distance = FVector::Distance(CurrentGridPosition, GridPosition);
    if (Distance > MovementRange)
    {
        return false;
    }

    // Move to the new position
    FVector WorldPosition = GridPosition * 100.0f; // Convert grid to world space
    FVector Direction = (WorldPosition - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction, 1.0f);

    // Update grid position and action points
    CurrentGridPosition = GridPosition;
    RemainingActionPoints--;

    // Set up movement completion callback
    FTimerHandle MovementTimer;
    GetWorld()->GetTimerManager().SetTimer(
        MovementTimer,
        this,
        &AEchoesCharacter::OnMovementComplete,
        0.5f,
        false
    );

    return true;
}

bool AEchoesCharacter::UseAbility(int32 AbilityIndex, const FVector& TargetPosition)
{
    if (RemainingActionPoints <= 0)
    {
        return false;
    }

    // Generate resonance based on archetype and subclass
    switch (BaseArchetype)
    {
        case EBaseArchetype::Weaver:
            GenerateResonance(EResonanceType::Faith);
            break;
        case EBaseArchetype::Scholar:
            GenerateResonance(EResonanceType::Curiosity);
            break;
        case EBaseArchetype::Warrior:
            GenerateResonance(EResonanceType::Faith);
            break;
        case EBaseArchetype::Mystic:
            GenerateResonance(EResonanceType::Doubt);
            break;
        case EBaseArchetype::Guardian:
            GenerateResonance(EResonanceType::Faith);
            break;
        case EBaseArchetype::Trickster:
            GenerateResonance(EResonanceType::Doubt);
            break;
        case EBaseArchetype::Sage:
            GenerateResonance(EResonanceType::Curiosity);
            break;
        case EBaseArchetype::Herald:
            GenerateResonance(EResonanceType::Faith);
            break;
        case EBaseArchetype::Architect:
            GenerateResonance(EResonanceType::Curiosity);
            break;
        case EBaseArchetype::Wanderer:
            GenerateResonance(EResonanceType::Curiosity);
            break;
    }

    RemainingActionPoints--;
    OnAbilityComplete();
    return true;
}

void AEchoesCharacter::GenerateResonance(EResonanceType Type, float Intensity)
{
    if (ResonanceComponent && CurrentResonance < ResonanceCapacity)
    {
        // Check cooldown
        float CurrentTime = GetWorld()->GetTimeSeconds();
        if (CurrentTime - LastResonanceTime >= ResonanceCooldown)
        {
            ResonanceComponent->GenerateResonance(Type, Intensity);
            CurrentResonance += Intensity;
            LastResonanceTime = CurrentTime;
        }
    }
}

void AEchoesCharacter::LevelUp()
{
    Level++;
    
    // Increase stats based on archetype and subclass
    switch (BaseArchetype)
    {
        case EBaseArchetype::Weaver:
            ResonanceCapacity += 10.0f;
            break;
        case EBaseArchetype::Scholar:
            ResonanceCapacity += 15.0f;
            break;
        case EBaseArchetype::Warrior:
            Health += 20.0f;
            break;
        case EBaseArchetype::Mystic:
            ResonanceCapacity += 12.0f;
            break;
        case EBaseArchetype::Guardian:
            Health += 15.0f;
            break;
        case EBaseArchetype::Trickster:
            MovementRange += 1;
            break;
        case EBaseArchetype::Sage:
            ResonanceCapacity += 10.0f;
            break;
        case EBaseArchetype::Herald:
            ResonanceCapacity += 8.0f;
            break;
        case EBaseArchetype::Architect:
            ResonanceCapacity += 15.0f;
            break;
        case EBaseArchetype::Wanderer:
            MovementRange += 1;
            break;
    }
}

void AEchoesCharacter::EvolveSubclass(ESubclass NewSubclass)
{
    // Check if the new subclass is compatible with the current archetype
    bool IsCompatible = false;
    
    // Check compatibility based on archetype
    switch (BaseArchetype)
    {
        case EBaseArchetype::Weaver:
            IsCompatible = (NewSubclass >= ESubclass::ThreadBinder && NewSubclass <= ESubclass::TapestryMender);
            break;
        case EBaseArchetype::Scholar:
            IsCompatible = (NewSubclass >= ESubclass::LoreKeeper && NewSubclass <= ESubclass::KnowledgeForge);
            break;
        case EBaseArchetype::Warrior:
            IsCompatible = (NewSubclass >= ESubclass::BladeDancer && NewSubclass <= ESubclass::HonorGuard);
            break;
        case EBaseArchetype::Mystic:
            IsCompatible = (NewSubclass >= ESubclass::DreamWalker && NewSubclass <= ESubclass::EssenceBinder);
            break;
        case EBaseArchetype::Guardian:
            IsCompatible = (NewSubclass >= ESubclass::BalanceKeeper && NewSubclass <= ESubclass::RealityWarden);
            break;
        case EBaseArchetype::Trickster:
            IsCompatible = (NewSubclass >= ESubclass::ShadowDancer && NewSubclass <= ESubclass::RealityJester);
            break;
        case EBaseArchetype::Sage:
            IsCompatible = (NewSubclass >= ESubclass::WisdomBearer && NewSubclass <= ESubclass::TruthSpeaker);
            break;
        case EBaseArchetype::Herald:
            IsCompatible = (NewSubclass >= ESubclass::DivineMessenger && NewSubclass <= ESubclass::CosmicHerald);
            break;
        case EBaseArchetype::Architect:
            IsCompatible = (NewSubclass >= ESubclass::RealityShaper && NewSubclass <= ESubclass::CosmicDesigner);
            break;
        case EBaseArchetype::Wanderer:
            IsCompatible = (NewSubclass >= ESubclass::Pathfinder && NewSubclass <= ESubclass::CosmicExplorer);
            break;
    }

    if (IsCompatible)
    {
        CurrentSubclass = NewSubclass;
        // TODO: Apply subclass-specific bonuses and abilities
    }
}

void AEchoesCharacter::OnMovementComplete()
{
    // TODO: Handle movement completion effects
}

void AEchoesCharacter::OnAbilityComplete()
{
    // TODO: Handle ability completion effects
}

void AEchoesCharacter::Interact()
{
    // Simple interaction implementation
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * InteractionRange;

    // Perform a line trace to find interactable objects
    FHitResult HitResult;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
    {
        // TODO: Handle interaction with hit object
    }
}

void AEchoesCharacter::DetectEchoes()
{
    if (EchoComponent)
    {
        EchoComponent->DetectEchoes(EchoDetectionRange);
    }
} 