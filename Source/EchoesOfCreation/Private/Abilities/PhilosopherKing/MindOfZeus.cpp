#include "Abilities/PhilosopherKing/MindOfZeus.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AMindOfZeus::AMindOfZeus()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
}

void AMindOfZeus::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    KnowledgeRange = 600.0f;
    ResonanceGenerationRate = 1.8f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.8f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.2f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 1.8f);
}

void AMindOfZeus::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateKnowledgeState(DeltaTime);
        CheckTargetValidity();
    }
}

void AMindOfZeus::InitializeKnowledge(EKnowledgeType KnowledgeType, float BasePower, float Duration)
{
    CurrentKnowledgeType = KnowledgeType;
    KnowledgePower = BasePower;
    KnowledgeDuration = Duration;

    // Adjust properties based on knowledge type
    switch (KnowledgeType)
    {
        case EKnowledgeType::DivineWisdom:
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.5f;
            break;
        case EKnowledgeType::CosmicInsight:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.0f;
            break;
        case EKnowledgeType::EternalTruth:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.8f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
            break;
    }
}

void AMindOfZeus::ActivateKnowledge()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnKnowledgeActivated();
    }
}

void AMindOfZeus::DeactivateKnowledge()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnKnowledgeDeactivated();
        CurrentTarget = nullptr;
    }
}

void AMindOfZeus::TargetKnowledge(AActor* Target)
{
    if (!bIsActive || !Target) return;

    // Check if target is in range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    if (Distance > KnowledgeRange) return;

    CurrentTarget = Target;
    float EnlightenmentStrength = CalculateEnlightenmentStrength(Target);
    OnTargetEnlightened(Target, EnlightenmentStrength);
    ApplyKnowledgeEffects(Target);
}

void AMindOfZeus::ApplyKnowledgeEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float EnlightenmentStrength = CalculateEnlightenmentStrength(Target);

        switch (CurrentKnowledgeType)
        {
            case EKnowledgeType::DivineWisdom:
                // Apply Divine Wisdom effects (enhanced perception and clarity)
                UGameplayStatics::ApplyDamage(Target, KnowledgePower * 0.1f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                }
                break;

            case EKnowledgeType::CosmicInsight:
                // Apply Cosmic Insight effects (reality manipulation)
                UGameplayStatics::ApplyDamage(Target, KnowledgePower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.3f;
                }
                break;

            case EKnowledgeType::EternalTruth:
                // Apply Eternal Truth effects (ultimate enlightenment)
                if (EnlightenmentStrength > 0.8f)
                {
                    // Strong enlightenment, apply significant effects
                    UGameplayStatics::ApplyDamage(Target, KnowledgePower * 0.2f, nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.4f;
                    }
                }
                else
                {
                    // Weak enlightenment, apply moderate effects
                    UGameplayStatics::ApplyDamage(Target, KnowledgePower * 0.1f, nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                    }
                }
                break;
        }
    }
}

float AMindOfZeus::CalculateEnlightenmentStrength(AActor* Target)
{
    // This is a placeholder for a more complex enlightenment strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's mental state
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental factors
    // - Distance from knowledge source

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void AMindOfZeus::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = KnowledgePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AMindOfZeus::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * KnowledgePower);
        }
    }
}

void AMindOfZeus::UpdateKnowledgeState(float DeltaTime)
{
    // Update knowledge duration
    KnowledgeDuration -= DeltaTime;
    if (KnowledgeDuration <= 0.0f)
    {
        DeactivateKnowledge();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target
    if (CurrentTarget)
    {
        ApplyKnowledgeEffects(CurrentTarget);
    }
}

void AMindOfZeus::CheckTargetValidity()
{
    if (!CurrentTarget || CurrentTarget->IsPendingKill())
    {
        CurrentTarget = nullptr;
        return;
    }

    // Check if target is still in range
    float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());
    if (Distance > KnowledgeRange)
    {
        CurrentTarget = nullptr;
    }
} 