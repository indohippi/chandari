#include "Abilities/CovenantWeaver/DivineNameResonance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ADivineNameResonance::ADivineNameResonance()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    ResonanceLocation = FVector::ZeroVector;
}

void ADivineNameResonance::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceRadius = 400.0f;
    ResonanceGenerationRate = 2.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.2f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.8f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.0f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.2f);
}

void ADivineNameResonance::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateResonanceState(DeltaTime);
        CheckAffectedActors();
    }
}

void ADivineNameResonance::InitializeResonance(EResonanceType NameType, float BasePower, float Duration)
{
    CurrentNameType = NameType;
    ResonancePower = BasePower;
    ResonanceDuration = Duration;

    // Adjust properties based on name type
    switch (NameType)
    {
        case EResonanceType::Tetragrammaton:
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.8f;
            break;
        case EResonanceType::Elohim:
            ResonanceModifiers[EResonanceType::Doubt] = 2.5f;
            EchoInteractions[EEchoType::Corrupted] = 2.8f;
            break;
        case EResonanceType::ShemHaMephorash:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.0f;
            EchoInteractions[EEchoType::Warped] = 3.2f;
            break;
    }
}

void ADivineNameResonance::ActivateResonance()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnResonanceActivated();
    }
}

void ADivineNameResonance::DeactivateResonance()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnResonanceDeactivated();
        ResonanceLocation = FVector::ZeroVector;
    }
}

void ADivineNameResonance::ChannelResonance(FVector Location)
{
    if (!bIsActive) return;

    ResonanceLocation = Location;
    float ResonanceStrength = CalculateResonanceStrength(nullptr);
    OnResonanceChanneled(Location, ResonanceStrength);
    CheckAffectedActors();
}

void ADivineNameResonance::ApplyResonanceEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float ResonanceStrength = CalculateResonanceStrength(Target);

        switch (CurrentNameType)
        {
            case EResonanceType::Tetragrammaton:
                // Apply Tetragrammaton effects
                UGameplayStatics::ApplyDamage(Target, -ResonancePower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                }
                break;

            case EResonanceType::Elohim:
                // Apply Elohim effects
                UGameplayStatics::ApplyDamage(Target, ResonancePower * 0.2f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f;
                }
                break;

            case EResonanceType::ShemHaMephorash:
                // Apply Shem HaMephorash effects
                if (ResonanceStrength > 0.7f)
                {
                    // Strong resonance, apply healing
                    UGameplayStatics::ApplyDamage(Target, -ResonancePower * 0.1f, nullptr, this, nullptr);
                }
                else
                {
                    // Weak resonance, apply damage
                    UGameplayStatics::ApplyDamage(Target, ResonancePower * 0.1f, nullptr, this, nullptr);
                }
                // Apply Shem HaMephorash effect (to be implemented in character class)
                break;
        }
    }
}

float ADivineNameResonance::CalculateResonanceStrength(AActor* Target)
{
    // This is a placeholder for a more complex resonance strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's divine connection strength
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental divine energy
    // - Distance from resonance center

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void ADivineNameResonance::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ResonancePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void ADivineNameResonance::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ResonancePower);
        }
    }
}

void ADivineNameResonance::UpdateResonanceState(float DeltaTime)
{
    // Update resonance duration
    ResonanceDuration -= DeltaTime;
    if (ResonanceDuration <= 0.0f)
    {
        DeactivateResonance();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to affected actors
    CheckAffectedActors();
}

void ADivineNameResonance::CheckAffectedActors()
{
    if (!bIsActive || ResonanceLocation == FVector::ZeroVector) return;

    // Find all actors in range
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, ResonanceLocation, ResonanceRadius, ActorsInRange);

    // Apply effects to each actor
    for (AActor* Actor : ActorsInRange)
    {
        if (Actor && !Actor->IsPendingKill())
        {
            ApplyResonanceEffects(Actor);
        }
    }
} 