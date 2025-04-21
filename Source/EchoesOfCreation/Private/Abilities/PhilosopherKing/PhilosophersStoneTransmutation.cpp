#include "Abilities/PhilosopherKing/PhilosophersStoneTransmutation.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

APhilosophersStoneTransmutation::APhilosophersStoneTransmutation()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
    TransmutationRange = 800.0f;
}

void APhilosophersStoneTransmutation::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.5f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 2.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 3.0f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.0f);
    EchoInteractions.Add(EEchoType::Corrupted, 2.5f);
    EchoInteractions.Add(EEchoType::Warped, 3.0f);
}

void APhilosophersStoneTransmutation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateTransmutationState(DeltaTime);
        if (CurrentTarget)
        {
            if (!CheckTargetValidity(CurrentTarget))
            {
                DeactivateTransmutation();
            }
            else
            {
                TransformMatter(CurrentTarget, DeltaTime);
            }
        }
    }
}

void APhilosophersStoneTransmutation::InitializeTransmutation(ETransmutationType TransmutationType, float BasePower, float Duration)
{
    CurrentTransmutationType = TransmutationType;
    TransmutationPower = BasePower;
    TransmutationDuration = Duration;

    // Adjust properties based on transmutation type
    switch (TransmutationType)
    {
        case ETransmutationType::PrimaMateria:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.5f;
            EchoInteractions[EEchoType::Warped] = 3.5f;
            break;
        case ETransmutationType::MagnumOpus:
            ResonanceModifiers[EResonanceType::Faith] = 3.0f;
            EchoInteractions[EEchoType::Divine] = 3.0f;
            break;
        case ETransmutationType::PhilosophersStone:
            ResonanceModifiers[EResonanceType::Doubt] = 3.2f;
            EchoInteractions[EEchoType::Corrupted] = 3.2f;
            break;
    }
}

void APhilosophersStoneTransmutation::ActivateTransmutation()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnTransmutationActivated();
    }
}

void APhilosophersStoneTransmutation::DeactivateTransmutation()
{
    if (bIsActive)
    {
        bIsActive = false;
        CurrentTarget = nullptr;
        OnTransmutationDeactivated();
    }
}

void APhilosophersStoneTransmutation::TargetTransmutation(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    CurrentTarget = Target;
    float TransmutationStrength = CalculateTransmutationStrength(Target);
    ApplyTransmutationEffects(Target);
    OnTargetTransmuted(Target, TransmutationStrength);
}

void APhilosophersStoneTransmutation::ApplyTransmutationEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float TransmutationStrength = CalculateTransmutationStrength(Target);

        switch (CurrentTransmutationType)
        {
            case ETransmutationType::PrimaMateria:
                // Apply prima materia effects (fundamental transformation)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f; // Speed boost
                }
                break;

            case ETransmutationType::MagnumOpus:
                // Apply magnum opus effects (perfection and enhancement)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.4f; // Greater speed boost
                }
                break;

            case ETransmutationType::PhilosophersStone:
                // Apply philosopher's stone effects (ultimate transformation)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f; // Speed reduction
                }
                break;
        }
    }
}

float APhilosophersStoneTransmutation::CalculateTransmutationStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / TransmutationRange), 0.0f, 1.0f);

    // Apply transmutation type modifier
    float TypeModifier = 1.0f;
    switch (CurrentTransmutationType)
    {
        case ETransmutationType::PrimaMateria:
            TypeModifier = 1.3f;
            break;
        case ETransmutationType::MagnumOpus:
            TypeModifier = 1.5f;
            break;
        case ETransmutationType::PhilosophersStone:
            TypeModifier = 1.7f;
            break;
    }

    return TransmutationPower * DistanceFactor * TypeModifier;
}

void APhilosophersStoneTransmutation::TransformMatter(AActor* Target, float DeltaTime)
{
    if (!Target) return;

    float TransmutationStrength = CalculateTransmutationStrength(Target);
    float EffectAmount = TransmutationStrength * DeltaTime;

    switch (CurrentTransmutationType)
    {
        case ETransmutationType::PrimaMateria:
            // Apply prima materia transformation effects over time
            UGameplayStatics::ApplyDamage(Target, -EffectAmount * 0.3f, nullptr, this, nullptr);
            break;

        case ETransmutationType::MagnumOpus:
            // Apply magnum opus transformation effects over time
            UGameplayStatics::ApplyDamage(Target, -EffectAmount * 0.5f, nullptr, this, nullptr);
            break;

        case ETransmutationType::PhilosophersStone:
            // Apply philosopher's stone transformation effects over time
            UGameplayStatics::ApplyDamage(Target, EffectAmount * 0.4f, nullptr, this, nullptr);
            break;
    }
}

void APhilosophersStoneTransmutation::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = TransmutationPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void APhilosophersStoneTransmutation::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * TransmutationPower);
        }
    }
}

void APhilosophersStoneTransmutation::UpdateTransmutationState(float DeltaTime)
{
    // Update transmutation duration
    TransmutationDuration -= DeltaTime;
    if (TransmutationDuration <= 0.0f)
    {
        DeactivateTransmutation();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target if valid
    if (CurrentTarget && CheckTargetValidity(CurrentTarget))
    {
        ApplyTransmutationEffects(CurrentTarget);
    }
}

bool APhilosophersStoneTransmutation::CheckTargetValidity(AActor* Target)
{
    if (!Target || Target->IsPendingKill()) return false;

    // Check if target is within range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    return Distance <= TransmutationRange;
} 