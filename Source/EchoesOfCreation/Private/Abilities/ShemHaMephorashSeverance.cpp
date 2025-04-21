#include "Abilities/ShemHaMephorashSeverance.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AShemHaMephorashSeverance::AShemHaMephorashSeverance()
{
    // Set ability properties
    AbilityName = TEXT("Shem HaMephorash Severance");
    ResonanceCost = 30.0f;
    Cooldown = 20.0f;
    Range = 700.0f;
    SelectedSefirah = EShemType::Keter; // Default to Keter
}

void AShemHaMephorashSeverance::SelectSefirah(EShemType Sefirah)
{
    SelectedSefirah = Sefirah;
}

bool AShemHaMephorashSeverance::ActivateAbility(const FVector& TargetLocation)
{
    if (!ABaseAbility::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the severance field
    CreateSeveranceField(TargetLocation);

    // Generate resonance based on the selected Sefirah
    switch (SelectedSefirah)
    {
        case EShemType::Keter:
            GenerateResonance(EResonanceType::Faith, 1.5f);
            break;
        case EShemType::Chokmah:
            GenerateResonance(EResonanceType::Faith, 1.2f);
            break;
        case EShemType::Binah:
            GenerateResonance(EResonanceType::Faith, 1.1f);
            break;
        case EShemType::Chesed:
            GenerateResonance(EResonanceType::Faith, 1.0f);
            break;
        case EShemType::Geburah:
            GenerateResonance(EResonanceType::Faith, 1.3f);
            break;
        case EShemType::Tiferet:
            GenerateResonance(EResonanceType::Faith, 1.4f);
            break;
        case EShemType::Netzach:
            GenerateResonance(EResonanceType::Faith, 0.9f);
            break;
        case EShemType::Hod:
            GenerateResonance(EResonanceType::Faith, 0.8f);
            break;
        case EShemType::Yesod:
            GenerateResonance(EResonanceType::Faith, 1.0f);
            break;
        case EShemType::Malkuth:
            GenerateResonance(EResonanceType::Faith, 0.7f);
            break;
    }

    return true;
}

void AShemHaMephorashSeverance::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the severance radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= SeveranceRadius)
            {
                ApplySeveranceEffects(Character);
            }
        }
    }

    // Set up the severance duration timer
    GetWorld()->GetTimerManager().SetTimer(
        SeveranceTimer,
        this,
        &AShemHaMephorashSeverance::OnSeveranceComplete,
        SeveranceDuration,
        false
    );
}

void AShemHaMephorashSeverance::CreateSeveranceField(const FVector& Center)
{
    // TODO: Implement visual effects for the severance field
    // This would include:
    // - A radiant field with the Sefirah's symbol
    // - Divine energy effects based on the selected Sefirah
    // - Reality distortion effects
}

void AShemHaMephorashSeverance::ApplySeveranceEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    // Apply effects based on the selected Sefirah
    switch (SelectedSefirah)
    {
        case EShemType::Keter:
            // Ultimate authority - powerful effects on all
            Target->GenerateResonance(EResonanceType::Faith, DivineIntensity * 2.0f);
            break;

        case EShemType::Chokmah:
            // Divine insight - reveal hidden truths
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement truth revelation effects
            }
            break;

        case EShemType::Binah:
            // Divine knowledge - enhance abilities
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement ability enhancement
            }
            break;

        case EShemType::Chesed:
            // Divine love - heal and protect
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(DivineIntensity * 25.0f);
            }
            break;

        case EShemType::Geburah:
            // Divine judgment - punish enemies
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                Target->ModifyHealth(-DivineIntensity * 20.0f);
            }
            break;

        case EShemType::Tiferet:
            // Divine harmony - balance effects
            Target->GenerateResonance(EResonanceType::Faith, DivineIntensity);
            break;

        case EShemType::Netzach:
            // Divine endurance - enhance stamina
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement stamina enhancement
            }
            break;

        case EShemType::Hod:
            // Divine splendor - enhance abilities
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement ability enhancement
            }
            break;

        case EShemType::Yesod:
            // Divine connection - link effects
            if (Target->GetTeam() == OwnerCharacter->GetTeam())
            {
                // TODO: Implement connection effects
            }
            break;

        case EShemType::Malkuth:
            // Divine manifestation - physical effects
            if (Target->GetTeam() != OwnerCharacter->GetTeam())
            {
                // TODO: Implement physical effects
            }
            break;
    }
}

void AShemHaMephorashSeverance::OnSeveranceComplete()
{
    // TODO: Clean up any ongoing effects
    // This could include:
    // - Removing visual effects
    // - Ending any ongoing status effects
    // - Reverting any temporary modifications
} 