#include "Abilities/JadeDragon/YinYangShift.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AYinYangShift::AYinYangShift()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
}

void AYinYangShift::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    BalanceRange = 800.0f;
    ResonanceGenerationRate = 1.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 0.8f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 0.8f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 0.7f);
    EchoInteractions.Add(EEchoType::Corrupted, 0.7f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);
}

void AYinYangShift::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateBalanceState(DeltaTime);
        CheckTargetValidity();
    }
}

void AYinYangShift::InitializeBalance(EBalanceType BalanceType, float BasePower, float Duration)
{
    CurrentBalanceType = BalanceType;
    BalancePower = BasePower;
    BalanceDuration = Duration;

    // Adjust properties based on balance type
    switch (BalanceType)
    {
        case EBalanceType::YinDominance:
            ResonanceModifiers[EResonanceType::Doubt] = 1.5f;
            EchoInteractions[EEchoType::Corrupted] = 1.8f;
            break;
        case EBalanceType::YangDominance:
            ResonanceModifiers[EResonanceType::Faith] = 1.5f;
            EchoInteractions[EEchoType::Divine] = 1.8f;
            break;
        case EBalanceType::PerfectBalance:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.0f;
            EchoInteractions[EEchoType::Warped] = 2.2f;
            break;
    }
}

void AYinYangShift::ActivateBalance()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnBalanceActivated();
    }
}

void AYinYangShift::DeactivateBalance()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnBalanceDeactivated();
        CurrentTarget = nullptr;
    }
}

void AYinYangShift::ShiftBalance(AActor* Target)
{
    if (!Target || !bIsActive) return;

    // Check if target is in range
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
        if (Distance <= BalanceRange)
        {
            CurrentTarget = Target;
            float BalanceValue = CalculateBalanceValue(Target);
            OnBalanceShifted(Target, BalanceValue);
            ApplyBalanceEffects(Target);
        }
    }
}

void AYinYangShift::ApplyBalanceEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float BalanceValue = CalculateBalanceValue(Target);

        switch (CurrentBalanceType)
        {
            case EBalanceType::YinDominance:
                // Apply Yin effects (darkness, cold, passivity)
                if (BalanceValue > 0.5f)
                {
                    // Target is too Yang, apply Yin correction
                    UGameplayStatics::ApplyDamage(Target, BalancePower * (BalanceValue - 0.5f), nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f;
                    }
                }
                else
                {
                    // Target is already Yin, enhance Yin
                    UGameplayStatics::ApplyDamage(Target, -BalancePower * (0.5f - BalanceValue), nullptr, this, nullptr);
                }
                break;

            case EBalanceType::YangDominance:
                // Apply Yang effects (light, heat, activity)
                if (BalanceValue < 0.5f)
                {
                    // Target is too Yin, apply Yang correction
                    UGameplayStatics::ApplyDamage(Target, BalancePower * (0.5f - BalanceValue), nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                    }
                }
                else
                {
                    // Target is already Yang, enhance Yang
                    UGameplayStatics::ApplyDamage(Target, -BalancePower * (BalanceValue - 0.5f), nullptr, this, nullptr);
                }
                break;

            case EBalanceType::PerfectBalance:
                // Apply perfect balance effects
                float Deviation = FMath::Abs(BalanceValue - 0.5f);
                if (Deviation > 0.2f)
                {
                    // Target is significantly imbalanced, apply correction
                    UGameplayStatics::ApplyDamage(Target, BalancePower * Deviation, nullptr, this, nullptr);
                }
                else
                {
                    // Target is close to balance, enhance harmony
                    UGameplayStatics::ApplyDamage(Target, -BalancePower * (0.2f - Deviation), nullptr, this, nullptr);
                }
                break;
        }
    }
}

float AYinYangShift::CalculateBalanceValue(AActor* Target)
{
    // This is a placeholder for a more complex balance calculation
    // In a real implementation, this would consider various factors like:
    // - Target's current state (health, energy, etc.)
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental factors

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void AYinYangShift::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = BalancePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AYinYangShift::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * BalancePower);
        }
    }
}

void AYinYangShift::UpdateBalanceState(float DeltaTime)
{
    // Update balance duration
    BalanceDuration -= DeltaTime;
    if (BalanceDuration <= 0.0f)
    {
        DeactivateBalance();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target
    if (CurrentTarget)
    {
        ApplyBalanceEffects(CurrentTarget);
    }
}

void AYinYangShift::CheckTargetValidity()
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
        if (Distance > BalanceRange)
        {
            CurrentTarget = nullptr;
        }
    }
} 