#include "Abilities/QliphothSeal.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AQliphothSeal::AQliphothSeal()
{
    // Set ability properties
    AbilityName = TEXT("Qliphoth Seal");
    ResonanceCost = 25.0f;
    Cooldown = 12.0f;
    Range = 500.0f;
    SelectedType = EQliphothType::Thaumiel; // Default to Thaumiel
}

void AQliphothSeal::SelectQliphothType(EQliphothType Type)
{
    SelectedType = Type;
}

bool AQliphothSeal::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the seal
    CreateSeal(TargetLocation);

    // Generate resonance based on the selected Qliphoth type
    switch (SelectedType)
    {
        case EQliphothType::Thaumiel:
            GenerateResonance(EResonanceType::Doubt, 1.0f);
            break;
        case EQliphothType::Ghagiel:
            GenerateResonance(EResonanceType::Doubt, 1.2f);
            break;
        case EQliphothType::Sathariel:
            GenerateResonance(EResonanceType::Doubt, 0.8f);
            break;
        case EQliphothType::Gamchicoth:
            GenerateResonance(EResonanceType::Doubt, 1.1f);
            break;
        case EQliphothType::Golachab:
            GenerateResonance(EResonanceType::Doubt, 0.9f);
            break;
    }

    return true;
}

void AQliphothSeal::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the seal radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= SealRadius)
            {
                ApplySealEffects(Character);
            }
        }
    }

    // Set up the seal duration timer
    GetWorld()->GetTimerManager().SetTimer(
        SealTimer,
        this,
        &AQliphothSeal::OnSealComplete,
        SealDuration,
        false
    );
}

void AQliphothSeal::CreateSeal(const FVector& Center)
{
    // TODO: Implement visual effects for the seal
    // This would include:
    // - A dark, swirling seal with the Qliphoth symbol
    // - Corrupted energy effects based on the selected type
    // - Seal boundary effects
}

void AQliphothSeal::ApplySealEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected Qliphoth type
    switch (SelectedType)
    {
        case EQliphothType::Thaumiel:
            // Duality and corruption - split effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-CorruptionIntensity * 10.0f);
                Target->GenerateResonance(EResonanceType::Doubt, CorruptionIntensity);
            }
            break;

        case EQliphothType::Ghagiel:
            // Chaos and disruption - area control
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement chaos effects (random movement, ability disruption)
            }
            break;

        case EQliphothType::Sathariel:
            // Concealment and deception - stealth effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement stealth effects
            }
            break;

        case EQliphothType::Gamchicoth:
            // Destruction and decay - damage over time
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement damage over time effects
            }
            break;

        case EQliphothType::Golachab:
            // Burning and purification - cleansing effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement cleansing effects
            }
            break;
    }
}

void AQliphothSeal::OnSealComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 