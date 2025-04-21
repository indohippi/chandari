#include "Abilities/ChakravartiAvatar/AvatarsStrike.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AAvatarsStrike::AAvatarsStrike()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
    StrikeRange = 1000.0f;
}

void AAvatarsStrike::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.5f);
    ResonanceModifiers[EResonanceType::Doubt] = 1.8f;
    ResonanceModifiers[EResonanceType::Curiosity] = 2.2f;

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.8f);
    EchoInteractions.Add(EEchoType::Warped, 2.2f);
}

void AAvatarsStrike::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateStrikeState(DeltaTime);
        if (CurrentTarget)
        {
            if (!CheckTargetValidity(CurrentTarget))
            {
                DeactivateStrike();
            }
        }
    }
}

void AAvatarsStrike::InitializeStrike(EStrikeType StrikeType, float BasePower, float Duration)
{
    CurrentStrikeType = StrikeType;
    StrikePower = BasePower;
    StrikeDuration = Duration;

    // Adjust properties based on strike type
    switch (StrikeType)
    {
        case EStrikeType::CosmicBlade:
            ResonanceModifiers[EResonanceType::Faith] = 3.0f;
            EchoInteractions[EEchoType::Divine] = 3.0f;
            break;
        case EStrikeType::DivineHammer:
            ResonanceModifiers[EResonanceType::Doubt] = 2.5f;
            EchoInteractions[EEchoType::Corrupted] = 2.5f;
            break;
        case EStrikeType::EternalSpear:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.2f;
            EchoInteractions[EEchoType::Warped] = 3.0f;
            break;
    }
}

void AAvatarsStrike::ActivateStrike()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnStrikeActivated();
    }
}

void AAvatarsStrike::DeactivateStrike()
{
    if (bIsActive)
    {
        bIsActive = false;
        CurrentTarget = nullptr;
        OnStrikeDeactivated();
    }
}

void AAvatarsStrike::StrikeTarget(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    CurrentTarget = Target;
    float StrikeStrength = CalculateStrikeStrength(Target);
    ApplyStrikeEffects(Target);
    OnTargetStruck(Target, StrikeStrength);
}

void AAvatarsStrike::ApplyStrikeEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float StrikeStrength = CalculateStrikeStrength(Target);

        switch (CurrentStrikeType)
        {
            case EStrikeType::CosmicBlade:
                // Apply Cosmic Blade effects (precision damage and faith resonance)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.9f; // Slight slow
                }
                break;

            case EStrikeType::DivineHammer:
                // Apply Divine Hammer effects (area damage and doubt resonance)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f; // Moderate slow
                }
                break;

            case EStrikeType::EternalSpear:
                // Apply Eternal Spear effects (piercing damage and curiosity resonance)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.7f; // Significant slow
                }
                break;
        }
    }
}

float AAvatarsStrike::CalculateStrikeStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / StrikeRange), 0.0f, 1.0f);

    // Apply strike type modifier
    float TypeModifier = 1.0f;
    switch (CurrentStrikeType)
    {
        case EStrikeType::CosmicBlade:
            TypeModifier = 1.2f;
            break;
        case EStrikeType::DivineHammer:
            TypeModifier = 1.5f;
            break;
        case EStrikeType::EternalSpear:
            TypeModifier = 1.3f;
            break;
    }

    return StrikePower * DistanceFactor * TypeModifier;
}

void AAvatarsStrike::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = StrikePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AAvatarsStrike::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * StrikePower);
        }
    }
}

void AAvatarsStrike::UpdateStrikeState(float DeltaTime)
{
    // Update strike duration
    StrikeDuration -= DeltaTime;
    if (StrikeDuration <= 0.0f)
    {
        DeactivateStrike();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target if valid
    if (CurrentTarget && CheckTargetValidity(CurrentTarget))
    {
        ApplyStrikeEffects(CurrentTarget);
    }
}

bool AAvatarsStrike::CheckTargetValidity(AActor* Target)
{
    if (!Target || Target->IsPendingKill()) return false;

    // Check if target is within range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    return Distance <= StrikeRange;
} 