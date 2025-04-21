#include "Abilities/AvatarsStrike.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AAvatarsStrike::AAvatarsStrike()
{
    // Set ability properties
    AbilityName = TEXT("Avatar's Strike");
    ResonanceCost = 40.0f;
    Cooldown = 25.0f;
    Range = 300.0f;
    SelectedType = EStrikeType::Divine; // Default to Divine
}

void AAvatarsStrike::SelectStrikeType(EStrikeType Type)
{
    SelectedType = Type;
}

bool AAvatarsStrike::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the strike effect
    CreateStrikeEffect(TargetLocation);

    // Generate resonance based on the selected strike type
    switch (SelectedType)
    {
        case EStrikeType::Divine:
            GenerateResonance(EResonanceType::Faith, 1.3f);
            break;
        case EStrikeType::Cosmic:
            GenerateResonance(EResonanceType::Faith, 1.4f);
            break;
        case EStrikeType::Karmic:
            GenerateResonance(EResonanceType::Doubt, 1.2f);
            break;
        case EStrikeType::Dharma:
            GenerateResonance(EResonanceType::Faith, 1.5f);
            break;
        case EStrikeType::Avatar:
            GenerateResonance(EResonanceType::Faith, 1.6f);
            break;
    }

    return true;
}

void AAvatarsStrike::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the strike radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= StrikeRadius)
            {
                ApplyStrikeEffects(Character);
            }
        }
    }
}

void AAvatarsStrike::CreateStrikeEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the strike
    // This would include:
    // - A powerful divine impact effect
    // - Visual effects based on the selected strike type
    // - Shockwave effects
}

void AAvatarsStrike::ApplyStrikeEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Calculate the damage based on the strike type and target
    float Damage = CalculateStrikeDamage(Target);

    // Apply effects based on the selected strike type
    switch (SelectedType)
    {
        case EStrikeType::Divine:
            // Pure divine energy - direct damage
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-Damage);
                Target->GenerateResonance(EResonanceType::Faith, DivinePower * 0.5f);
            }
            break;

        case EStrikeType::Cosmic:
            // Cosmic force - area damage
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-Damage * 1.2f);
                Target->GenerateResonance(EResonanceType::Faith, DivinePower * 0.3f);
            }
            break;

        case EStrikeType::Karmic:
            // Karmic retribution - damage based on target's actions
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement karmic damage calculation
                Target->ModifyHealth(-Damage * 1.1f);
                Target->GenerateResonance(EResonanceType::Doubt, DivinePower);
            }
            break;

        case EStrikeType::Dharma:
            // Righteous judgment - balanced damage
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-Damage * 1.3f);
                Target->GenerateResonance(EResonanceType::Faith, DivinePower * 0.7f);
            }
            else
            {
                Target->ModifyHealth(Damage * 0.5f);
                Target->GenerateResonance(EResonanceType::Faith, DivinePower * 0.3f);
            }
            break;

        case EStrikeType::Avatar:
            // Ultimate avatar power - maximum damage
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-Damage * 1.5f);
                Target->GenerateResonance(EResonanceType::Faith, DivinePower);
            }
            else
            {
                Target->ModifyHealth(Damage * 0.7f);
                Target->GenerateResonance(EResonanceType::Faith, DivinePower * 0.5f);
            }
            break;
    }
}

float AAvatarsStrike::CalculateStrikeDamage(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    // Base damage calculation
    float Damage = StrikeDamage * DivinePower;

    // Apply modifiers based on strike type
    switch (SelectedType)
    {
        case EStrikeType::Divine:
            Damage *= 1.0f;
            break;
        case EStrikeType::Cosmic:
            Damage *= 1.2f;
            break;
        case EStrikeType::Karmic:
            Damage *= 1.1f;
            break;
        case EStrikeType::Dharma:
            Damage *= 1.3f;
            break;
        case EStrikeType::Avatar:
            Damage *= 1.5f;
            break;
    }

    return Damage;
} 