#include "Abilities/CovenantWeaver/QliphothSeal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AQliphothSeal::AQliphothSeal()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    SealLocation = FVector::ZeroVector;
}

void AQliphothSeal::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    SealRadius = 300.0f;
    ResonanceGenerationRate = 1.8f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.2f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 2.0f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 2.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);
}

void AQliphothSeal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateSealState(DeltaTime);
        CheckAffectedActors();
    }
}

void AQliphothSeal::InitializeSeal(ESealType SealType, float BasePower, float Duration)
{
    CurrentSealType = SealType;
    SealPower = BasePower;
    SealDuration = Duration;

    // Adjust properties based on seal type
    switch (SealType)
    {
        case ESealType::Thaumiel:
            ResonanceModifiers[EResonanceType::Faith] = 2.0f;
            EchoInteractions[EEchoType::Divine] = 2.2f;
            break;
        case ESealType::Gamaliel:
            ResonanceModifiers[EResonanceType::Doubt] = 2.5f;
            EchoInteractions[EEchoType::Corrupted] = 2.8f;
            break;
        case ESealType::Samael:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.2f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
            break;
    }
}

void AQliphothSeal::ActivateSeal()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnSealActivated();
    }
}

void AQliphothSeal::DeactivateSeal()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnSealDeactivated();
        SealLocation = FVector::ZeroVector;
    }
}

void AQliphothSeal::PlaceSeal(FVector Location)
{
    if (!bIsActive) return;

    SealLocation = Location;
    float SealStrength = CalculateSealStrength(nullptr);
    OnSealPlaced(Location, SealStrength);
    CheckAffectedActors();
}

void AQliphothSeal::ApplySealEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float SealStrength = CalculateSealStrength(Target);

        switch (CurrentSealType)
        {
            case ESealType::Thaumiel:
                // Apply Thaumiel effects (opposing forces)
                UGameplayStatics::ApplyDamage(Target, SealPower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.9f;
                }
                break;

            case ESealType::Gamaliel:
                // Apply Gamaliel effects (corruption)
                UGameplayStatics::ApplyDamage(Target, SealPower * 0.2f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.7f;
                }
                break;

            case ESealType::Samael:
                // Apply Samael effects (destruction)
                if (SealStrength > 0.8f)
                {
                    // Strong seal, apply heavy damage
                    UGameplayStatics::ApplyDamage(Target, SealPower * 0.3f, nullptr, this, nullptr);
                }
                else
                {
                    // Weak seal, apply moderate damage
                    UGameplayStatics::ApplyDamage(Target, SealPower * 0.15f, nullptr, this, nullptr);
                }
                // Apply Samael effect (to be implemented in character class)
                break;
        }
    }
}

float AQliphothSeal::CalculateSealStrength(AActor* Target)
{
    // This is a placeholder for a more complex seal strength calculation
    // In a real implementation, this would consider various factors like:
    // - Target's corruption level
    // - Target's resonance levels
    // - Target's echo interactions
    // - Environmental corruption
    // - Distance from seal center

    // For now, return a random value between 0 and 1
    return FMath::RandRange(0.0f, 1.0f);
}

void AQliphothSeal::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = SealPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AQliphothSeal::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * SealPower);
        }
    }
}

void AQliphothSeal::UpdateSealState(float DeltaTime)
{
    // Update seal duration
    SealDuration -= DeltaTime;
    if (SealDuration <= 0.0f)
    {
        DeactivateSeal();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to affected actors
    CheckAffectedActors();
}

void AQliphothSeal::CheckAffectedActors()
{
    if (!bIsActive || SealLocation == FVector::ZeroVector) return;

    // Find all actors in range
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, SealLocation, SealRadius, ActorsInRange);

    // Apply effects to each actor
    for (AActor* Actor : ActorsInRange)
    {
        if (Actor && !Actor->IsPendingKill())
        {
            ApplySealEffects(Actor);
        }
    }
} 