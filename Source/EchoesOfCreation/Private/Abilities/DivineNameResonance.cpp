#include "Abilities/DivineNameResonance.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ADivineNameResonance::ADivineNameResonance()
{
    // Set ability properties
    AbilityName = TEXT("Divine Name Resonance");
    ResonanceCost = 20.0f;
    Cooldown = 8.0f;
    Range = 600.0f;
    SelectedName = EDivineName::El; // Default to El
}

void ADivineNameResonance::SelectDivineName(EDivineName Name)
{
    SelectedName = Name;
}

bool ADivineNameResonance::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the resonance field
    CreateResonanceField(TargetLocation);

    // Generate resonance based on the selected name
    switch (SelectedName)
    {
        case EDivineName::El:
            GenerateResonance(EResonanceType::Faith, 0.8f);
            break;
        case EDivineName::Elohim:
            GenerateResonance(EResonanceType::Faith, 1.0f);
            break;
        case EDivineName::YHVH:
            GenerateResonance(EResonanceType::Faith, 1.2f);
            break;
        case EDivineName::Adonai:
            GenerateResonance(EResonanceType::Faith, 0.9f);
            break;
        case EDivineName::Shaddai:
            GenerateResonance(EResonanceType::Faith, 1.1f);
            break;
    }

    return true;
}

void ADivineNameResonance::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the resonance radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= ResonanceRadius)
            {
                ApplyResonanceEffects(Character);
            }
        }
    }

    // Set up the resonance duration timer
    GetWorld()->GetTimerManager().SetTimer(
        ResonanceTimer,
        this,
        &ADivineNameResonance::OnResonanceComplete,
        ResonanceDuration,
        false
    );
}

void ADivineNameResonance::CreateResonanceField(const FVector& Center)
{
    // TODO: Implement visual effects for the resonance field
    // This would include:
    // - A glowing field with the divine name's symbol
    // - Ethereal effects based on the selected name
    // - Resonance waves emanating from the center
}

void ADivineNameResonance::ApplyResonanceEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected divine name
    switch (SelectedName)
    {
        case EDivineName::El:
            // Mercy and compassion - heal allies
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(ResonanceIntensity * 20.0f);
            }
            break;

        case EDivineName::Elohim:
            // Justice and power - damage enemies
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-ResonanceIntensity * 15.0f);
            }
            break;

        case EDivineName::YHVH:
            // The ineffable name - powerful effects on all
            Target->GenerateResonance(EResonanceType::Faith, ResonanceIntensity);
            break;

        case EDivineName::Adonai:
            // Lordship and sovereignty - control effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement control effects
            }
            break;

        case EDivineName::Shaddai:
            // Almighty and protection - shield allies
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement shield effects
            }
            break;
    }
}

void ADivineNameResonance::OnResonanceComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 