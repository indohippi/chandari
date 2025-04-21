#include "Abilities/JadeDragon/QiConduit.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AQiConduit::AQiConduit()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
}

void AQiConduit::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ChannelRange = 600.0f;
    ResonanceGenerationRate = 1.2f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 0.8f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.2f);
    EchoInteractions.Add(EEchoType::Corrupted, 0.7f);
    EchoInteractions.Add(EEchoType::Warped, 1.8f);
}

void AQiConduit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateConduitState(DeltaTime);
        CheckTargetValidity();
    }
}

void AQiConduit::InitializeConduit(EQiType QiType, float BasePower, float Duration)
{
    CurrentQiType = QiType;
    QiPower = BasePower;
    ConduitDuration = Duration;

    // Adjust properties based on Qi type
    switch (QiType)
    {
        case EQiType::VitalFlow:
            ResonanceModifiers[EResonanceType::Faith] = 1.5f;
            EchoInteractions[EEchoType::Divine] = 1.8f;
            break;
        case EQiType::MeridianSurge:
            ResonanceModifiers[EResonanceType::Doubt] = 1.5f;
            EchoInteractions[EEchoType::Corrupted] = 1.8f;
            break;
        case EQiType::SpiritChannel:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.0f;
            EchoInteractions[EEchoType::Warped] = 2.2f;
            break;
    }
}

void AQiConduit::ActivateConduit()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnConduitActivated();
    }
}

void AQiConduit::DeactivateConduit()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnConduitDeactivated();
        CurrentTarget = nullptr;
    }
}

void AQiConduit::ChannelQi(AActor* Target)
{
    if (!Target || !bIsActive) return;

    // Check if target is in range
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
        if (Distance <= ChannelRange)
        {
            CurrentTarget = Target;
            float QiStrength = CalculateQiStrength(Target);
            OnQiChanneled(Target, QiStrength);
            ApplyQiEffects(Target);
        }
    }
}

void AQiConduit::ApplyQiEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float QiStrength = CalculateQiStrength(Target);

        switch (CurrentQiType)
        {
            case EQiType::VitalFlow:
                // Apply healing and vitality effects
                UGameplayStatics::ApplyDamage(Target, -QiPower * 0.1f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f;
                }
                break;

            case EQiType::MeridianSurge:
                // Apply energy surge and damage
                UGameplayStatics::ApplyDamage(Target, QiPower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.3f;
                }
                break;

            case EQiType::SpiritChannel:
                // Apply spiritual effects and balance
                if (QiStrength > 0.7f)
                {
                    // Strong Qi flow, apply healing
                    UGameplayStatics::ApplyDamage(Target, -QiPower * 0.05f, nullptr, this, nullptr);
                }
                else
                {
                    // Weak Qi flow, apply damage
                    UGameplayStatics::ApplyDamage(Target, QiPower * 0.05f, nullptr, this, nullptr);
                }
                // Apply spirit effect (to be implemented in character class)
                break;
        }
    }
}

float AQiConduit::CalculateQiStrength(AActor* Target)
{
    // This is a placeholder for a more complex Qi strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's current health and energy
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental Qi flow

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void AQiConduit::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = QiPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AQiConduit::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * QiPower);
        }
    }
}

void AQiConduit::UpdateConduitState(float DeltaTime)
{
    // Update conduit duration
    ConduitDuration -= DeltaTime;
    if (ConduitDuration <= 0.0f)
    {
        DeactivateConduit();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target
    if (CurrentTarget)
    {
        ApplyQiEffects(CurrentTarget);
    }
}

void AQiConduit::CheckTargetValidity()
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
        if (Distance > ChannelRange)
        {
            CurrentTarget = nullptr;
        }
    }
} 