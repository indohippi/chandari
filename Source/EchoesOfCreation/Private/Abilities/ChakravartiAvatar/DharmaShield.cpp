#include "Abilities/ChakravartiAvatar/DharmaShield.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ADharmaShield::ADharmaShield()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    ShieldRadius = 500.0f;
}

void ADharmaShield::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 1.5f;
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

void ADharmaShield::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateShieldState(DeltaTime);
        FindActorsInRadius();
        CheckProtectedActors();
    }
}

void ADharmaShield::InitializeShield(EDharmaType DharmaType, float BasePower, float Duration)
{
    CurrentDharmaType = DharmaType;
    ShieldPower = BasePower;
    ShieldDuration = Duration;

    // Adjust properties based on dharma type
    switch (DharmaType)
    {
        case EDharmaType::Protection:
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.5f;
            break;
        case EDharmaType::Justice:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.0f;
            break;
        case EDharmaType::Balance:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.8f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
            break;
    }
}

void ADharmaShield::ActivateShield()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnShieldActivated();
        FindActorsInRadius();
    }
}

void ADharmaShield::DeactivateShield()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnShieldDeactivated();
        ProtectedActors.Empty();
    }
}

void ADharmaShield::SetShieldRadius(float Radius)
{
    ShieldRadius = FMath::Max(100.0f, Radius);
}

void ADharmaShield::ApplyShieldEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float ProtectionStrength = CalculateProtectionStrength(Target);

        switch (CurrentDharmaType)
        {
            case EDharmaType::Protection:
                // Apply Protection effects (damage reduction and healing)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.9f; // Slow down for protection
                }
                break;

            case EDharmaType::Justice:
                // Apply Justice effects (damage reflection and retaliation)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f; // Speed up for justice
                }
                break;

            case EDharmaType::Balance:
                // Apply Balance effects (adaptive protection)
                if (ProtectionStrength > 0.8f)
                {
                    // Strong protection, apply significant effects
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 1.0f; // Maintain speed
                    }
                }
                else
                {
                    // Weak protection, apply moderate effects
                    if (Character->GetCharacterMovement())
                    {
                        Character->GetCharacterMovement()->MaxWalkSpeed *= 0.95f; // Slight slowdown
                    }
                }
                break;
        }
    }
}

float ADharmaShield::CalculateProtectionStrength(AActor* Target)
{
    // This is a placeholder for a more complex protection strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's alignment with dharma
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental factors
    // - Distance from shield center

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void ADharmaShield::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ShieldPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void ADharmaShield::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ShieldPower);
        }
    }
}

void ADharmaShield::UpdateShieldState(float DeltaTime)
{
    // Update shield duration
    ShieldDuration -= DeltaTime;
    if (ShieldDuration <= 0.0f)
    {
        DeactivateShield();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to all protected actors
    for (AActor* Actor : ProtectedActors)
    {
        ApplyShieldEffects(Actor);
    }
}

void ADharmaShield::CheckProtectedActors()
{
    // Remove invalid actors
    ProtectedActors.RemoveAll([](AActor* Actor) {
        return !Actor || Actor->IsPendingKill();
    });

    // Check if actors are still in range
    for (int32 i = ProtectedActors.Num() - 1; i >= 0; --i)
    {
        float Distance = FVector::Distance(GetActorLocation(), ProtectedActors[i]->GetActorLocation());
        if (Distance > ShieldRadius)
        {
            ProtectedActors.RemoveAt(i);
        }
    }
}

void ADharmaShield::FindActorsInRadius()
{
    TArray<AActor*> ActorsInRadius;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), ShieldRadius, ActorsInRadius);

    for (AActor* Actor : ActorsInRadius)
    {
        if (!ProtectedActors.Contains(Actor))
        {
            ProtectedActors.Add(Actor);
            float ProtectionStrength = CalculateProtectionStrength(Actor);
            OnActorProtected(Actor, ProtectionStrength);
        }
    }
} 