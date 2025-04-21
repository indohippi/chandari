#include "Abilities/BaseAbility.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"

ABaseAbility::ABaseAbility()
{
    PrimaryActorTick.bCanEverTick = true;
    LastActivationTime = 0.0f;
}

void ABaseAbility::InitializeAbility(AEchoesCharacter* Character)
{
    OwnerCharacter = Character;
}

bool ABaseAbility::ActivateAbility(const FVector& TargetLocation)
{
    if (!CanActivate())
    {
        return false;
    }

    // Check if target is in range
    float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), TargetLocation);
    if (Distance > Range)
    {
        return false;
    }

    // Apply effects
    ApplyEffects(TargetLocation);

    // Update last activation time
    LastActivationTime = GetWorld()->GetTimeSeconds();

    return true;
}

bool ABaseAbility::CanActivate() const
{
    if (!OwnerCharacter)
    {
        return false;
    }

    // Check cooldown
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastActivationTime < Cooldown)
    {
        return false;
    }

    // Check resonance cost
    if (OwnerCharacter->GetCurrentResonance() < ResonanceCost)
    {
        return false;
    }

    return true;
}

void ABaseAbility::ApplyEffects(const FVector& TargetLocation)
{
    // Base implementation does nothing
    // Override in derived classes
}

void ABaseAbility::GenerateResonance(EResonanceType Type, float Intensity)
{
    if (OwnerCharacter)
    {
        OwnerCharacter->GenerateResonance(Type, Intensity);
    }
} 