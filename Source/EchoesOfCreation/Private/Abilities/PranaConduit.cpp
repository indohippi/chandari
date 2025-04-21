#include "Abilities/PranaConduit.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

APranaConduit::APranaConduit()
{
    // Set ability properties
    AbilityName = TEXT("Prana Conduit");
    ResonanceCost = 35.0f;
    Cooldown = 30.0f;
    Range = 600.0f;
    SelectedType = EPranaType::Vital; // Default to Vital
}

void APranaConduit::SelectPranaType(EPranaType Type)
{
    SelectedType = Type;
}

bool APranaConduit::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the prana conduit
    CreatePranaConduit(TargetLocation);

    // Generate resonance based on the selected prana type
    switch (SelectedType)
    {
        case EPranaType::Vital:
            GenerateResonance(EResonanceType::Faith, 1.1f);
            break;
        case EPranaType::Spiritual:
            GenerateResonance(EResonanceType::Faith, 1.3f);
            break;
        case EPranaType::Cosmic:
            GenerateResonance(EResonanceType::Faith, 1.4f);
            break;
        case EPranaType::Karmic:
            GenerateResonance(EResonanceType::Doubt, 1.2f);
            break;
        case EPranaType::Divine:
            GenerateResonance(EResonanceType::Faith, 1.5f);
            break;
    }

    return true;
}

void APranaConduit::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the conduit radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= ConduitRadius)
            {
                ApplyPranaEffects(Character);
            }
        }
    }

    // Set up the conduit duration timer
    GetWorld()->GetTimerManager().SetTimer(
        ConduitTimer,
        this,
        &APranaConduit::OnConduitComplete,
        ConduitDuration,
        false
    );
}

void APranaConduit::CreatePranaConduit(const FVector& Center)
{
    // TODO: Implement visual effects for the prana conduit
    // This would include:
    // - A flowing energy conduit
    // - Visual effects based on the selected prana type
    // - Energy flow patterns
}

void APranaConduit::ApplyPranaEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Calculate the prana flow based on the target and prana type
    float PranaFlow = CalculatePranaFlow(Target);

    // Apply effects based on the selected prana type
    switch (SelectedType)
    {
        case EPranaType::Vital:
            // Life force and healing - heal allies, drain enemies
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(PranaFlow * 20.0f);
                Target->GenerateResonance(EResonanceType::Faith, PranaIntensity);
            }
            else
            {
                Target->ModifyHealth(-PranaFlow * 15.0f);
            }
            break;

        case EPranaType::Spiritual:
            // Spiritual energy and enlightenment - enhance abilities
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement ability enhancement
                Target->GenerateResonance(EResonanceType::Faith, PranaIntensity * 1.2f);
            }
            break;

        case EPranaType::Cosmic:
            // Cosmic energy and power - powerful effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(PranaFlow * 25.0f);
                Target->GenerateResonance(EResonanceType::Faith, PranaIntensity * 1.3f);
            }
            else
            {
                Target->ModifyHealth(-PranaFlow * 20.0f);
                Target->GenerateResonance(EResonanceType::Doubt, PranaIntensity * 0.5f);
            }
            break;

        case EPranaType::Karmic:
            // Karmic energy and balance - balanced effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(PranaFlow * 15.0f);
                Target->GenerateResonance(EResonanceType::Faith, PranaIntensity);
            }
            else
            {
                Target->ModifyHealth(-PranaFlow * 15.0f);
                Target->GenerateResonance(EResonanceType::Doubt, PranaIntensity);
            }
            break;

        case EPranaType::Divine:
            // Divine energy and transcendence - ultimate effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(PranaFlow * 30.0f);
                Target->GenerateResonance(EResonanceType::Faith, PranaIntensity * 1.5f);
            }
            else
            {
                Target->ModifyHealth(-PranaFlow * 25.0f);
                Target->GenerateResonance(EResonanceType::Doubt, PranaIntensity * 0.7f);
            }
            break;
    }
}

float APranaConduit::CalculatePranaFlow(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    // Base prana flow calculation
    float Flow = PranaIntensity;

    // Apply modifiers based on prana type
    switch (SelectedType)
    {
        case EPranaType::Vital:
            Flow *= 1.0f;
            break;
        case EPranaType::Spiritual:
            Flow *= 1.2f;
            break;
        case EPranaType::Cosmic:
            Flow *= 1.3f;
            break;
        case EPranaType::Karmic:
            Flow *= 1.1f;
            break;
        case EPranaType::Divine:
            Flow *= 1.5f;
            break;
    }

    return Flow;
}

void APranaConduit::OnConduitComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 