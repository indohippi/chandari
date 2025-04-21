#include "Abilities/CosmicBreath.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ACosmicBreath::ACosmicBreath()
{
    // Set ability properties
    AbilityName = TEXT("Cosmic Breath");
    ResonanceCost = 30.0f;
    Cooldown = 20.0f;
    Range = 500.0f;
    SelectedType = ECosmicBreathType::Solar; // Default to Solar
}

void ACosmicBreath::SelectBreathType(ECosmicBreathType Type)
{
    SelectedType = Type;
}

bool ACosmicBreath::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the cosmic breath effect
    CreateBreathEffect(TargetLocation);

    // Generate resonance based on the selected breath type
    switch (SelectedType)
    {
        case ECosmicBreathType::Solar:
            GenerateResonance(EResonanceType::Faith, 1.2f);
            break;
        case ECosmicBreathType::Lunar:
            GenerateResonance(EResonanceType::Curiosity, 1.1f);
            break;
        case ECosmicBreathType::Stellar:
            GenerateResonance(EResonanceType::Faith, 1.4f);
            break;
        case ECosmicBreathType::Void:
            GenerateResonance(EResonanceType::Doubt, 1.3f);
            break;
        case ECosmicBreathType::Celestial:
            GenerateResonance(EResonanceType::Faith, 1.5f);
            break;
    }

    return true;
}

void ACosmicBreath::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the breath radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= BreathRadius)
            {
                ApplyBreathEffects(Character);
            }
        }
    }

    // Set up the breath duration timer
    GetWorld()->GetTimerManager().SetTimer(
        BreathTimer,
        this,
        &ACosmicBreath::OnBreathComplete,
        BreathDuration,
        false
    );
}

void ACosmicBreath::CreateBreathEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the cosmic breath
    // This would include:
    // - A flowing cosmic energy effect
    // - Visual effects based on the selected breath type
    // - Particle systems for the breath
}

void ACosmicBreath::ApplyBreathEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected breath type
    switch (SelectedType)
    {
        case ECosmicBreathType::Solar:
            // Radiant energy and light - heal allies, damage enemies
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(CosmicIntensity * 25.0f);
                Target->GenerateResonance(EResonanceType::Faith, CosmicIntensity);
            }
            else
            {
                Target->ModifyHealth(-CosmicIntensity * 20.0f);
            }
            break;

        case ECosmicBreathType::Lunar:
            // Mystical and transformative - enhance abilities
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement ability enhancement
                Target->GenerateResonance(EResonanceType::Curiosity, CosmicIntensity);
            }
            break;

        case ECosmicBreathType::Stellar:
            // Cosmic power and creation - powerful effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(CosmicIntensity * 30.0f);
                Target->GenerateResonance(EResonanceType::Faith, CosmicIntensity * 1.2f);
            }
            else
            {
                Target->ModifyHealth(-CosmicIntensity * 25.0f);
            }
            break;

        case ECosmicBreathType::Void:
            // Dark energy and destruction - powerful damage
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-CosmicIntensity * 35.0f);
                Target->GenerateResonance(EResonanceType::Doubt, CosmicIntensity);
            }
            break;

        case ECosmicBreathType::Celestial:
            // Divine balance and harmony - balanced effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(CosmicIntensity * 20.0f);
                Target->GenerateResonance(EResonanceType::Faith, CosmicIntensity);
            }
            else
            {
                Target->ModifyHealth(-CosmicIntensity * 15.0f);
                Target->GenerateResonance(EResonanceType::Doubt, CosmicIntensity * 0.5f);
            }
            break;
    }
}

void ACosmicBreath::OnBreathComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 