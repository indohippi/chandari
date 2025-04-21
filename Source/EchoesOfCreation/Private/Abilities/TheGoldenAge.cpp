#include "Abilities/TheGoldenAge.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ATheGoldenAge::ATheGoldenAge()
{
    // Set ability properties
    AbilityName = TEXT("The Golden Age");
    ResonanceCost = 50.0f;
    Cooldown = 45.0f;
    Range = 1000.0f;
    SelectedAspect = EGoldenAgeAspect::Prosperity; // Default to Prosperity
}

void ATheGoldenAge::SelectGoldenAgeAspect(EGoldenAgeAspect Aspect)
{
    SelectedAspect = Aspect;
}

bool ATheGoldenAge::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the Golden Age field
    CreateGoldenAgeField(TargetLocation);

    // Generate resonance based on the selected aspect
    switch (SelectedAspect)
    {
        case EGoldenAgeAspect::Prosperity:
            GenerateResonance(EResonanceType::Faith, 1.5f);
            break;
        case EGoldenAgeAspect::Harmony:
            GenerateResonance(EResonanceType::Faith, 1.3f);
            break;
        case EGoldenAgeAspect::Knowledge:
            GenerateResonance(EResonanceType::Curiosity, 1.4f);
            break;
        case EGoldenAgeAspect::Justice:
            GenerateResonance(EResonanceType::Faith, 1.2f);
            break;
        case EGoldenAgeAspect::Unity:
            GenerateResonance(EResonanceType::Faith, 1.6f);
            break;
    }

    return true;
}

void ATheGoldenAge::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the Golden Age radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= GoldenAgeRadius)
            {
                ApplyGoldenAgeEffects(Character);
            }
        }
    }

    // Set up the Golden Age duration timer
    GetWorld()->GetTimerManager().SetTimer(
        GoldenAgeTimer,
        this,
        &ATheGoldenAge::OnGoldenAgeComplete,
        GoldenAgeDuration,
        false
    );
}

void ATheGoldenAge::CreateGoldenAgeField(const FVector& Center)
{
    // TODO: Implement visual effects for the Golden Age field
    // This would include:
    // - A radiant golden field
    // - Effects based on the selected aspect
    // - Symbols of the Golden Age
}

void ATheGoldenAge::ApplyGoldenAgeEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected aspect
    switch (SelectedAspect)
    {
        case EGoldenAgeAspect::Prosperity:
            // Abundance and growth - enhance all attributes
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(GoldenAgeIntensity * 30.0f);
                // TODO: Implement other attribute enhancements
            }
            break;

        case EGoldenAgeAspect::Harmony:
            // Peace and balance - heal and protect
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(GoldenAgeIntensity * 20.0f);
                // TODO: Implement protection effects
            }
            break;

        case EGoldenAgeAspect::Knowledge:
            // Wisdom and understanding - enhance abilities
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement ability enhancement
            }
            break;

        case EGoldenAgeAspect::Justice:
            // Fairness and order - balance effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-GoldenAgeIntensity * 15.0f);
            }
            else
            {
                Target->ModifyHealth(GoldenAgeIntensity * 15.0f);
            }
            break;

        case EGoldenAgeAspect::Unity:
            // Cooperation and strength - enhance all allies
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(GoldenAgeIntensity * 25.0f);
                Target->GenerateResonance(EResonanceType::Faith, GoldenAgeIntensity);
            }
            break;
    }
}

void ATheGoldenAge::OnGoldenAgeComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 