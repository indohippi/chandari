#include "Abilities/SerpentsWhisper/ScalesOfMaAt.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AScalesOfMaAt::AScalesOfMaAt()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
}

void AScalesOfMaAt::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    JudgmentRange = 1000.0f;
    ResonanceGenerationRate = 1.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.2f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 0.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 0.8f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 0.5f);
    EchoInteractions.Add(EEchoType::Warped, 0.8f);
}

void AScalesOfMaAt::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateScaleState(DeltaTime);
    }
}

void AScalesOfMaAt::InitializeScales(EScaleType ScaleType, float BasePower, float Duration)
{
    CurrentScaleType = ScaleType;
    ScalePower = BasePower;
    ScaleDuration = Duration;

    // Adjust properties based on scale type
    switch (ScaleType)
    {
        case EScaleType::BalanceJudgment:
            ResonanceModifiers[EResonanceType::Faith] = 1.5f;
            EchoInteractions[EEchoType::Divine] = 1.8f;
            break;
        case EScaleType::TruthRevelation:
            ResonanceModifiers[EResonanceType::Curiosity] = 1.8f;
            EchoInteractions[EEchoType::Warped] = 2.0f;
            break;
        case EScaleType::OrderEnforcement:
            ResonanceModifiers[EResonanceType::Doubt] = 0.2f;
            EchoInteractions[EEchoType::Corrupted] = 0.3f;
            break;
    }
}

void AScalesOfMaAt::ActivateScales()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnScalesActivated();
    }
}

void AScalesOfMaAt::DeactivateScales()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnScalesDeactivated();
    }
}

void AScalesOfMaAt::JudgeTarget(AActor* Target)
{
    if (!Target || !bIsActive) return;

    float JudgmentValue = CalculateJudgmentValue(Target);
    OnTargetJudged(Target, JudgmentValue);
    ApplyJudgmentEffects(Target);
}

void AScalesOfMaAt::ApplyJudgmentEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float JudgmentValue = CalculateJudgmentValue(Target);

        switch (CurrentScaleType)
        {
            case EScaleType::BalanceJudgment:
                // Apply effects based on balance
                if (JudgmentValue > 0.5f)
                {
                    // Target is too aligned with creation
                    UGameplayStatics::ApplyDamage(Target, ScalePower * (1.0f - JudgmentValue), nullptr, this, nullptr);
                }
                else
                {
                    // Target is too aligned with destruction
                    UGameplayStatics::ApplyDamage(Target, ScalePower * JudgmentValue, nullptr, this, nullptr);
                }
                break;

            case EScaleType::TruthRevelation:
                // Apply truth-based effects
                if (JudgmentValue < 0.3f)
                {
                    // Target is hiding something
                    UGameplayStatics::ApplyDamage(Target, ScalePower * 0.2f, nullptr, this, nullptr);
                    // Apply reveal effect (to be implemented in character class)
                }
                break;

            case EScaleType::OrderEnforcement:
                // Apply order-based effects
                if (JudgmentValue > 0.7f)
                {
                    // Target is too chaotic
                    UGameplayStatics::ApplyDamage(Target, ScalePower * 0.15f, nullptr, this, nullptr);
                    // Apply order effect (to be implemented in character class)
                }
                break;
        }
    }
}

float AScalesOfMaAt::CalculateJudgmentValue(AActor* Target)
{
    // This is a placeholder for a more complex judgment calculation
    // In a real implementation, this would consider various factors like:
    // - Target's alignment with creation/destruction
    // - Target's resonance levels
    // - Target's echo interactions
    // - Target's recent actions

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void AScalesOfMaAt::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ScalePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AScalesOfMaAt::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ScalePower);
        }
    }
}

void AScalesOfMaAt::UpdateScaleState(float DeltaTime)
{
    // Update scale duration
    ScaleDuration -= DeltaTime;
    if (ScaleDuration <= 0.0f)
    {
        DeactivateScales();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();
} 