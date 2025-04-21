#include "Abilities/PhilosophersStoneTransmutation.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

APhilosophersStoneTransmutation::APhilosophersStoneTransmutation()
{
    // Set ability properties
    AbilityName = TEXT("Philosopher's Stone Transmutation");
    ResonanceCost = 35.0f;
    Cooldown = 15.0f;
    Range = 600.0f;
    SelectedType = ETransmutationType::LeadToGold; // Default to basic transmutation
}

void APhilosophersStoneTransmutation::SelectTransmutationType(ETransmutationType Type)
{
    SelectedType = Type;
}

bool APhilosophersStoneTransmutation::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the transmutation field
    CreateTransmutationField(TargetLocation);

    // Generate resonance based on the selected transmutation type
    switch (SelectedType)
    {
        case ETransmutationType::LeadToGold:
            GenerateResonance(EResonanceType::Curiosity, 1.0f);
            break;
        case ETransmutationType::SilverToMercury:
            GenerateResonance(EResonanceType::Curiosity, 1.2f);
            break;
        case ETransmutationType::CopperToIron:
            GenerateResonance(EResonanceType::Curiosity, 0.9f);
            break;
        case ETransmutationType::TinToPlatinum:
            GenerateResonance(EResonanceType::Curiosity, 1.3f);
            break;
        case ETransmutationType::Quicksilver:
            GenerateResonance(EResonanceType::Curiosity, 1.1f);
            break;
    }

    return true;
}

void APhilosophersStoneTransmutation::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the transmutation radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= TransmutationRadius)
            {
                ApplyTransmutationEffects(Character);
            }
        }
    }

    // Set up the transmutation duration timer
    GetWorld()->GetTimerManager().SetTimer(
        TransmutationTimer,
        this,
        &APhilosophersStoneTransmutation::OnTransmutationComplete,
        TransmutationDuration,
        false
    );
}

void APhilosophersStoneTransmutation::CreateTransmutationField(const FVector& Center)
{
    // TODO: Implement visual effects for the transmutation field
    // This would include:
    // - A glowing alchemical circle
    // - Elemental symbols based on the selected type
    // - Transmutation energy effects
}

void APhilosophersStoneTransmutation::ApplyTransmutationEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected transmutation type
    switch (SelectedType)
    {
        case ETransmutationType::LeadToGold:
            // Basic transmutation - enhance attributes
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement attribute enhancement
            }
            break;

        case ETransmutationType::SilverToMercury:
            // Elemental shift - change properties
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement property alteration
            }
            break;

        case ETransmutationType::CopperToIron:
            // Metal transformation - strengthen or weaken
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(AlchemicalIntensity * 15.0f);
            }
            else
            {
                Target->ModifyHealth(-AlchemicalIntensity * 10.0f);
            }
            break;

        case ETransmutationType::TinToPlatinum:
            // Noble metal creation - powerful effects
            Target->GenerateResonance(EResonanceType::Curiosity, AlchemicalIntensity);
            break;

        case ETransmutationType::Quicksilver:
            // Liquid metal manipulation - control effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement control effects
            }
            break;
    }
}

void APhilosophersStoneTransmutation::OnTransmutationComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 