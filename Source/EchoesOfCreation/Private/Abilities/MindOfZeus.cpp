#include "Abilities/MindOfZeus.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMindOfZeus::AMindOfZeus()
{
    // Set ability properties
    AbilityName = TEXT("Mind of Zeus");
    ResonanceCost = 40.0f;
    Cooldown = 25.0f;
    Range = 800.0f;
    SelectedAspect = EZeusAspect::Thunder; // Default to Thunder
}

void AMindOfZeus::SelectZeusAspect(EZeusAspect Aspect)
{
    SelectedAspect = Aspect;
}

bool AMindOfZeus::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the divine field
    CreateDivineField(TargetLocation);

    // Generate resonance based on the selected aspect
    switch (SelectedAspect)
    {
        case EZeusAspect::Thunder:
            GenerateResonance(EResonanceType::Faith, 1.4f);
            break;
        case EZeusAspect::Wisdom:
            GenerateResonance(EResonanceType::Curiosity, 1.2f);
            break;
        case EZeusAspect::Justice:
            GenerateResonance(EResonanceType::Faith, 1.1f);
            break;
        case EZeusAspect::Authority:
            GenerateResonance(EResonanceType::Faith, 1.3f);
            break;
        case EZeusAspect::Storm:
            GenerateResonance(EResonanceType::Faith, 1.0f);
            break;
    }

    return true;
}

void AMindOfZeus::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the aspect radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= AspectRadius)
            {
                ApplyAspectEffects(Character);
            }
        }
    }

    // Set up the aspect duration timer
    GetWorld()->GetTimerManager().SetTimer(
        AspectTimer,
        this,
        &AMindOfZeus::OnAspectComplete,
        AspectDuration,
        false
    );
}

void AMindOfZeus::CreateDivineField(const FVector& Center)
{
    // TODO: Implement visual effects for the divine field
    // This would include:
    // - A radiant field with Zeus's symbol
    // - Divine energy effects based on the selected aspect
    // - Weather effects for Storm aspect
}

void AMindOfZeus::ApplyAspectEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected aspect
    switch (SelectedAspect)
    {
        case EZeusAspect::Thunder:
            // Lightning and power - damage enemies
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-DivinePower * 25.0f);
            }
            break;

        case EZeusAspect::Wisdom:
            // Divine knowledge - enhance abilities
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement ability enhancement
            }
            break;

        case EZeusAspect::Justice:
            // Divine judgment - balance effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-DivinePower * 15.0f);
            }
            else
            {
                Target->ModifyHealth(DivinePower * 10.0f);
            }
            break;

        case EZeusAspect::Authority:
            // Divine rule - control effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement control effects
            }
            break;

        case EZeusAspect::Storm:
            // Weather control - area effects
            Target->GenerateResonance(EResonanceType::Faith, DivinePower);
            break;
    }
}

void AMindOfZeus::OnAspectComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 