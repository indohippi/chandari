#include "Abilities/CovenantWeaver/ShemHaMephorashSeverance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AShemHaMephorashSeverance::AShemHaMephorashSeverance()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
    SeveranceRange = 1000.0f;
}

void AShemHaMephorashSeverance::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.5f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 3.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 2.0f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 3.0f);
    EchoInteractions.Add(EEchoType::Corrupted, 2.0f);
    EchoInteractions.Add(EEchoType::Warped, 2.5f);
}

void AShemHaMephorashSeverance::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateSeveranceState(DeltaTime);
        if (CurrentTarget)
        {
            if (!CheckTargetValidity(CurrentTarget))
            {
                DeactivateSeverance();
            }
            else
            {
                ModifyReality(CurrentTarget, DeltaTime);
            }
        }
    }
}

void AShemHaMephorashSeverance::InitializeSeverance(ESeveranceType SeveranceType, float BasePower, float Duration)
{
    CurrentSeveranceType = SeveranceType;
    SeverancePower = BasePower;
    SeveranceDuration = Duration;

    // Adjust properties based on severance type
    switch (SeveranceType)
    {
        case ESeveranceType::NameOfCreation:
            ResonanceModifiers[EResonanceType::Faith] = 3.5f;
            EchoInteractions[EEchoType::Divine] = 3.5f;
            break;
        case ESeveranceType::NameOfDestruction:
            ResonanceModifiers[EResonanceType::Doubt] = 3.0f;
            EchoInteractions[EEchoType::Corrupted] = 3.0f;
            break;
        case ESeveranceType::NameOfBalance:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.2f;
            EchoInteractions[EEchoType::Warped] = 3.2f;
            break;
    }
}

void AShemHaMephorashSeverance::ActivateSeverance()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnSeveranceActivated();
    }
}

void AShemHaMephorashSeverance::DeactivateSeverance()
{
    if (bIsActive)
    {
        bIsActive = false;
        CurrentTarget = nullptr;
        OnSeveranceDeactivated();
    }
}

void AShemHaMephorashSeverance::SeverTarget(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    CurrentTarget = Target;
    float SeveranceStrength = CalculateSeveranceStrength(Target);
    ApplySeveranceEffects(Target);
    OnTargetSevered(Target, SeveranceStrength);
}

void AShemHaMephorashSeverance::ApplySeveranceEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float SeveranceStrength = CalculateSeveranceStrength(Target);

        switch (CurrentSeveranceType)
        {
            case ESeveranceType::NameOfCreation:
                // Apply creation effects (healing and enhancement)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f; // Speed boost
                }
                break;

            case ESeveranceType::NameOfDestruction:
                // Apply destruction effects (damage and weakening)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f; // Speed reduction
                }
                break;

            case ESeveranceType::NameOfBalance:
                // Apply balance effects (stabilization and harmony)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.0f; // Maintain speed
                }
                break;
        }
    }
}

float AShemHaMephorashSeverance::CalculateSeveranceStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / SeveranceRange), 0.0f, 1.0f);

    // Apply severance type modifier
    float TypeModifier = 1.0f;
    switch (CurrentSeveranceType)
    {
        case ESeveranceType::NameOfCreation:
            TypeModifier = 1.4f;
            break;
        case ESeveranceType::NameOfDestruction:
            TypeModifier = 1.6f;
            break;
        case ESeveranceType::NameOfBalance:
            TypeModifier = 1.2f;
            break;
    }

    return SeverancePower * DistanceFactor * TypeModifier;
}

void AShemHaMephorashSeverance::ModifyReality(AActor* Target, float DeltaTime)
{
    if (!Target) return;

    float SeveranceStrength = CalculateSeveranceStrength(Target);
    float EffectAmount = SeveranceStrength * DeltaTime;

    switch (CurrentSeveranceType)
    {
        case ESeveranceType::NameOfCreation:
            // Apply creation effects over time
            UGameplayStatics::ApplyDamage(Target, -EffectAmount, nullptr, this, nullptr);
            break;

        case ESeveranceType::NameOfDestruction:
            // Apply destruction effects over time
            UGameplayStatics::ApplyDamage(Target, EffectAmount, nullptr, this, nullptr);
            break;

        case ESeveranceType::NameOfBalance:
            // Apply balance effects over time
            UGameplayStatics::ApplyDamage(Target, -EffectAmount * 0.5f, nullptr, this, nullptr);
            break;
    }
}

void AShemHaMephorashSeverance::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = SeverancePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AShemHaMephorashSeverance::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * SeverancePower);
        }
    }
}

void AShemHaMephorashSeverance::UpdateSeveranceState(float DeltaTime)
{
    // Update severance duration
    SeveranceDuration -= DeltaTime;
    if (SeveranceDuration <= 0.0f)
    {
        DeactivateSeverance();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target if valid
    if (CurrentTarget && CheckTargetValidity(CurrentTarget))
    {
        ApplySeveranceEffects(CurrentTarget);
    }
}

bool AShemHaMephorashSeverance::CheckTargetValidity(AActor* Target)
{
    if (!Target || Target->IsPendingKill()) return false;

    // Check if target is within range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    return Distance <= SeveranceRange;
} 