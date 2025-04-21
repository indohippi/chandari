#include "Abilities/SerpentsWhisper/SerpentsEmbrace.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ASerpentsEmbrace::ASerpentsEmbrace()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
}

void ASerpentsEmbrace::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    EmbraceRange = 800.0f;
    ResonanceGenerationRate = 1.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 0.8f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.2f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 0.7f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.8f);
    EchoInteractions.Add(EEchoType::Warped, 1.3f);
}

void ASerpentsEmbrace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateEmbraceState(DeltaTime);
        CheckTargetValidity();
    }
}

void ASerpentsEmbrace::InitializeEmbrace(EEmbraceType EmbraceType, float BasePower, float Duration)
{
    CurrentEmbraceType = EmbraceType;
    EmbracePower = BasePower;
    EmbraceDuration = Duration;

    // Adjust properties based on embrace type
    switch (EmbraceType)
    {
        case EEmbraceType::CreationCoil:
            ResonanceModifiers[EResonanceType::Faith] = 1.5f;
            EchoInteractions[EEchoType::Divine] = 1.8f;
            break;
        case EEmbraceType::DestructionFang:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.2f;
            break;
        case EEmbraceType::EternalCycle:
            ResonanceModifiers[EResonanceType::Curiosity] = 1.8f;
            EchoInteractions[EEchoType::Warped] = 2.0f;
            break;
    }
}

void ASerpentsEmbrace::ActivateEmbrace()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnEmbraceActivated();
    }
}

void ASerpentsEmbrace::DeactivateEmbrace()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnEmbraceDeactivated();
        CurrentTarget = nullptr;
    }
}

void ASerpentsEmbrace::TargetEmbrace(AActor* Target)
{
    if (!Target || !bIsActive) return;

    // Check if target is in range
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
        if (Distance <= EmbraceRange)
        {
            CurrentTarget = Target;
            OnTargetEmbraced(Target);
            ApplyEmbraceEffects(Target);
        }
    }
}

void ASerpentsEmbrace::ApplyEmbraceEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        switch (CurrentEmbraceType)
        {
            case EEmbraceType::CreationCoil:
                // Apply healing and creation effects
                UGameplayStatics::ApplyDamage(Target, -EmbracePower * 0.1f, nullptr, this, nullptr);
                // Apply creation effect (to be implemented in character class)
                break;

            case EEmbraceType::DestructionFang:
                // Apply damage and destruction effects
                UGameplayStatics::ApplyDamage(Target, EmbracePower * 0.15f, nullptr, this, nullptr);
                // Apply destruction effect (to be implemented in character class)
                break;

            case EEmbraceType::EternalCycle:
                // Apply cycle effects (both creation and destruction)
                float CycleValue = FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f);
                if (CycleValue > 0)
                {
                    // Creation phase
                    UGameplayStatics::ApplyDamage(Target, -EmbracePower * 0.05f, nullptr, this, nullptr);
                }
                else
                {
                    // Destruction phase
                    UGameplayStatics::ApplyDamage(Target, EmbracePower * 0.05f, nullptr, this, nullptr);
                }
                // Apply cycle effect (to be implemented in character class)
                break;
        }
    }
}

void ASerpentsEmbrace::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = EmbracePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void ASerpentsEmbrace::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * EmbracePower);
        }
    }
}

void ASerpentsEmbrace::UpdateEmbraceState(float DeltaTime)
{
    // Update embrace duration
    EmbraceDuration -= DeltaTime;
    if (EmbraceDuration <= 0.0f)
    {
        DeactivateEmbrace();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target
    if (CurrentTarget)
    {
        ApplyEmbraceEffects(CurrentTarget);
    }
}

void ASerpentsEmbrace::CheckTargetValidity()
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
        if (Distance > EmbraceRange)
        {
            CurrentTarget = nullptr;
        }
    }
} 