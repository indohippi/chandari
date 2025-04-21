#include "Abilities/PhilosopherKing/GoldenAge.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AGoldenAge::AGoldenAge()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    AgeRadius = 800.0f;
}

void AGoldenAge::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.2f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.8f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 1.8f);
}

void AGoldenAge::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateAgeState(DeltaTime);
        FindActorsInRadius();
        CheckAffectedActors();
    }
}

void AGoldenAge::InitializeAge(EAgeType AgeType, float BasePower, float Duration)
{
    CurrentAgeType = AgeType;
    AgePower = BasePower;
    AgeDuration = Duration;

    // Adjust properties based on age type
    switch (AgeType)
    {
        case EAgeType::Harmony:
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.8f;
            break;
        case EAgeType::Prosperity:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.0f;
            break;
        case EAgeType::Perfection:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.0f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
            break;
    }
}

void AGoldenAge::ActivateAge()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnAgeActivated();
        FindActorsInRadius();
    }
}

void AGoldenAge::DeactivateAge()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnAgeDeactivated();
        AffectedActors.Empty();
    }
}

void AGoldenAge::SetAgeRadius(float Radius)
{
    AgeRadius = FMath::Max(100.0f, Radius);
}

void AGoldenAge::ApplyAgeEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float HarmonyStrength = CalculateHarmonyStrength(Target);

        switch (CurrentAgeType)
        {
            case EAgeType::Harmony:
                // Apply Harmony effects (balance and stability)
                UGameplayStatics::ApplyDamage(Target, AgePower * 0.1f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                }
                break;

            case EAgeType::Prosperity:
                // Apply Prosperity effects (growth and enhancement)
                UGameplayStatics::ApplyDamage(Target, AgePower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.3f;
                }
                break;

            case EAgeType::Perfection:
                // Apply Perfection effects (ultimate state)
                if (HarmonyStrength > 0.8f)
                {
                    // Strong harmony, apply significant effects
                    UGameplayStatics::ApplyDamage(Target, AgePower * 0.2f, nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.4f;
                    }
                }
                else
                {
                    // Weak harmony, apply moderate effects
                    UGameplayStatics::ApplyDamage(Target, AgePower * 0.1f, nullptr, this, nullptr);
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f;
                    }
                }
                break;
        }
    }
}

float AGoldenAge::CalculateHarmonyStrength(AActor* Target)
{
    // This is a placeholder for a more complex harmony strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's alignment with the age type
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental factors
    // - Distance from age center

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void AGoldenAge::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = AgePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AGoldenAge::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * AgePower);
        }
    }
}

void AGoldenAge::UpdateAgeState(float DeltaTime)
{
    // Update age duration
    AgeDuration -= DeltaTime;
    if (AgeDuration <= 0.0f)
    {
        DeactivateAge();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to all affected actors
    for (AActor* Actor : AffectedActors)
    {
        ApplyAgeEffects(Actor);
    }
}

void AGoldenAge::CheckAffectedActors()
{
    // Remove invalid actors
    AffectedActors.RemoveAll([](AActor* Actor) {
        return !Actor || Actor->IsPendingKill();
    });

    // Check if actors are still in range
    for (int32 i = AffectedActors.Num() - 1; i >= 0; --i)
    {
        float Distance = FVector::Distance(GetActorLocation(), AffectedActors[i]->GetActorLocation());
        if (Distance > AgeRadius)
        {
            AffectedActors.RemoveAt(i);
        }
    }
}

void AGoldenAge::FindActorsInRadius()
{
    TArray<AActor*> ActorsInRadius;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), AgeRadius, ActorsInRadius);

    for (AActor* Actor : ActorsInRadius)
    {
        if (!AffectedActors.Contains(Actor))
        {
            AffectedActors.Add(Actor);
            float HarmonyStrength = CalculateHarmonyStrength(Actor);
            OnActorAffected(Actor, HarmonyStrength);
        }
    }
} 