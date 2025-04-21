#include "Abilities/CovenantWeaver/DivineNameResonance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ADivineNameResonance::ADivineNameResonance()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
    ResonanceRange = 1000.0f;
}

void ADivineNameResonance::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 3.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 3.5f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 4.0f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.0f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);
}

void ADivineNameResonance::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateResonanceState(DeltaTime);
        if (CurrentTarget)
        {
            if (!CheckTargetValidity(CurrentTarget))
            {
                DeactivateResonance();
            }
            else
            {
                AmplifyDivineName(CurrentTarget, DeltaTime);
            }
        }
    }
}

void ADivineNameResonance::InitializeResonance(EResonanceType ResonanceType, float BasePower, float Duration)
{
    CurrentResonanceType = ResonanceType;
    ResonancePower = BasePower;
    ResonanceDuration = Duration;

    // Adjust properties based on resonance type
    switch (ResonanceType)
    {
        case EResonanceType::Divine:
            ResonanceModifiers[EResonanceType::Faith] = 4.0f;
            EchoInteractions[EEchoType::Divine] = 4.5f;
            break;
        case EResonanceType::Celestial:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.5f;
            EchoInteractions[EEchoType::Warped] = 3.0f;
            break;
        case EResonanceType::Sacred:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.5f;
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
        CurrentTarget = nullptr;
        OnResonanceDeactivated();
    }
}

void ADivineNameResonance::TargetResonance(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    CurrentTarget = Target;
    float ResonanceStrength = CalculateResonanceStrength(Target);
    ApplyResonanceEffects(Target);
    OnTargetResonated(Target, ResonanceStrength);
}

void ADivineNameResonance::ApplyResonanceEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float ResonanceStrength = CalculateResonanceStrength(Target);

        switch (CurrentResonanceType)
        {
            case EResonanceType::Divine:
                // Apply divine effects (enhancement and protection)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.3f; // Speed boost
                }
                break;

            case EResonanceType::Celestial:
                // Apply celestial effects (balance and harmony)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f; // Moderate speed boost
                }
                break;

            case EResonanceType::Sacred:
                // Apply sacred effects (purification and cleansing)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.9f; // Speed reduction
                }
                break;
        }
    }
}

float ADivineNameResonance::CalculateResonanceStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ResonanceRange), 0.0f, 1.0f);

    // Apply resonance type modifier
    float TypeModifier = 1.0f;
    switch (CurrentResonanceType)
    {
        case EResonanceType::Divine:
            TypeModifier = 1.5f;
            break;
        case EResonanceType::Celestial:
            TypeModifier = 1.3f;
            break;
        case EResonanceType::Sacred:
            TypeModifier = 1.2f;
            break;
    }

    return ResonancePower * DistanceFactor * TypeModifier;
}

void ADivineNameResonance::AmplifyDivineName(AActor* Target, float DeltaTime)
{
    if (!Target) return;

    float ResonanceStrength = CalculateResonanceStrength(Target);
    float EffectAmount = ResonanceStrength * DeltaTime;

    switch (CurrentResonanceType)
    {
        case EResonanceType::Divine:
            // Apply divine amplification effects over time
            UGameplayStatics::ApplyDamage(Target, -EffectAmount, nullptr, this, nullptr);
            break;

        case EResonanceType::Celestial:
            // Apply celestial amplification effects over time
            UGameplayStatics::ApplyDamage(Target, -EffectAmount * 0.7f, nullptr, this, nullptr);
            break;

        case EResonanceType::Sacred:
            // Apply sacred amplification effects over time
            UGameplayStatics::ApplyDamage(Target, EffectAmount * 0.5f, nullptr, this, nullptr);
            break;
    }
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

    // Apply effects to current target if valid
    if (CurrentTarget && CheckTargetValidity(CurrentTarget))
    {
        ApplyResonanceEffects(CurrentTarget);
    }
}

bool ADivineNameResonance::CheckTargetValidity(AActor* Target)
{
    if (!Target || Target->IsPendingKill()) return false;

    // Check if target is within range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    return Distance <= ResonanceRange;
} 