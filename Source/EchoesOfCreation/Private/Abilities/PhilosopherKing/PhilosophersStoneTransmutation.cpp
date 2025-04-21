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
}

void APhilosophersStoneTransmutation::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    TransmutationRange = 500.0f;
    ResonanceGenerationRate = 1.5f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.2f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.0f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.8f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.2f);
}

void APhilosophersStoneTransmutation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateTransmutationState(DeltaTime);
        CheckTargetValidity();
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
            ResonanceModifiers[EResonanceType::Faith] = 1.8f;
            EchoInteractions[EEchoType::Divine] = 2.0f;
            break;
        case ETransmutationType::MagnumOpus:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.2f;
            break;
        case ETransmutationType::PhilosophersStone:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.5f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
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
        OnTransmutationDeactivated();
        CurrentTarget = nullptr;
    }
}

void APhilosophersStoneTransmutation::TargetTransmutation(AActor* Target)
{
    if (!bIsActive || !Target) return;

    // Check if target is in range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    if (Distance > TransmutationRange) return;

    CurrentTarget = Target;
    float TransmutationStrength = CalculateTransmutationStrength(Target);
    OnTargetTransmuted(Target, TransmutationStrength);
    ApplyTransmutationEffects(Target);
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
                // Apply Prima Materia effects (purification and healing)
                UGameplayStatics::ApplyDamage(Target, TransmutationPower * 0.1f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f;
                }
                break;

            case ETransmutationType::MagnumOpus:
                // Apply Magnum Opus effects (transformation and enhancement)
                UGameplayStatics::ApplyDamage(Target, TransmutationPower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                }
                break;

            case ETransmutationType::PhilosophersStone:
                // Apply Philosopher's Stone effects (ultimate transformation)
                if (TransmutationStrength > 0.8f)
                {
                    // Strong transmutation, apply significant effects
                    UGameplayStatics::ApplyDamage(Target, TransmutationPower * 0.2f, nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.3f;
                    }
                }
                else
                {
                    // Weak transmutation, apply moderate effects
                    UGameplayStatics::ApplyDamage(Target, TransmutationPower * 0.1f, nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f;
                    }
                }
                break;
        }
    }
}

float APhilosophersStoneTransmutation::CalculateTransmutationStrength(AActor* Target)
{
    // This is a placeholder for a more complex transmutation strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's material composition
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental factors
    // - Distance from transmutation source

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
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

    // Apply effects to current target
    if (CurrentTarget)
    {
        ApplyTransmutationEffects(CurrentTarget);
    }
}

void APhilosophersStoneTransmutation::CheckTargetValidity()
{
    if (!CurrentTarget || CurrentTarget->IsPendingKill())
    {
        CurrentTarget = nullptr;
        return;
    }

    // Check if target is still in range
    float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());
    if (Distance > TransmutationRange)
    {
        CurrentTarget = nullptr;
    }
} 