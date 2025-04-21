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
}

void AShemHaMephorashSeverance::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    SeveranceRange = 500.0f;
    ResonanceGenerationRate = 1.5f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.2f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.8f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.8f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);
}

void AShemHaMephorashSeverance::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateSeveranceState(DeltaTime);
        CheckTargetValidity();
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
        case ESeveranceType::NameSeverance:
            ResonanceModifiers[EResonanceType::Faith] = 2.0f;
            EchoInteractions[EEchoType::Divine] = 2.2f;
            break;
        case ESeveranceType::DivineCut:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.2f;
            break;
        case ESeveranceType::SacredRending:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.5f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
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
        OnSeveranceDeactivated();
        CurrentTarget = nullptr;
    }
}

void AShemHaMephorashSeverance::TargetSeverance(AActor* Target)
{
    if (!Target || !bIsActive) return;

    // Check if target is in range
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
        if (Distance <= SeveranceRange)
        {
            CurrentTarget = Target;
            float SeveranceStrength = CalculateSeveranceStrength(Target);
            OnTargetSevered(Target, SeveranceStrength);
            ApplySeveranceEffects(Target);
        }
    }
}

void AShemHaMephorashSeverance::ApplySeveranceEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float SeveranceStrength = CalculateSeveranceStrength(Target);

        switch (CurrentSeveranceType)
        {
            case ESeveranceType::NameSeverance:
                // Apply divine name severance effects
                UGameplayStatics::ApplyDamage(Target, SeverancePower * 0.2f, nullptr, this, nullptr);
                // Apply divine name effect (to be implemented in character class)
                break;

            case ESeveranceType::DivineCut:
                // Apply divine cut effects
                UGameplayStatics::ApplyDamage(Target, SeverancePower * 0.3f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.7f;
                }
                break;

            case ESeveranceType::SacredRending:
                // Apply sacred rending effects
                if (SeveranceStrength > 0.8f)
                {
                    // Strong severance, apply heavy damage
                    UGameplayStatics::ApplyDamage(Target, SeverancePower * 0.4f, nullptr, this, nullptr);
                }
                else
                {
                    // Weak severance, apply moderate damage
                    UGameplayStatics::ApplyDamage(Target, SeverancePower * 0.2f, nullptr, this, nullptr);
                }
                // Apply sacred rending effect (to be implemented in character class)
                break;
        }
    }
}

float AShemHaMephorashSeverance::CalculateSeveranceStrength(AActor* Target)
{
    // This is a placeholder for a more complex severance strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's divine connection strength
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental divine energy

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
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

    // Apply effects to current target
    if (CurrentTarget)
    {
        ApplySeveranceEffects(CurrentTarget);
    }
}

void AShemHaMephorashSeverance::CheckTargetValidity()
{
    if (!CurrentTarget || CurrentTarget->IsPendingKill())
    {
        CurrentTarget = nullptr;
        return;
    }

    // Check if target is still in range
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), CurrentTarget->GetActorLocation());
        if (Distance > SeveranceRange)
        {
            CurrentTarget = nullptr;
        }
    }
} 