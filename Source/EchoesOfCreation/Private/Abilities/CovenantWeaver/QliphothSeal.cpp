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
    SealRadius = 500.0f;
}

void AQliphothSeal::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.5f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 3.0f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.0f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.0f);
    EchoInteractions.Add(EEchoType::Corrupted, 3.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.5f);
}

void AQliphothSeal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateSealState(DeltaTime);
        CheckSealedActors();
        ProcessSealedActors(DeltaTime);
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
        case ESealType::Containment:
            ResonanceModifiers[EResonanceType::Doubt] = 3.5f;
            EchoInteractions[EEchoType::Corrupted] = 4.0f;
            break;
        case ESealType::Purification:
            ResonanceModifiers[EResonanceType::Faith] = 3.0f;
            EchoInteractions[EEchoType::Divine] = 3.5f;
            break;
        case ESealType::Transformation:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.2f;
            EchoInteractions[EEchoType::Warped] = 3.2f;
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
        SealLocation = FVector::ZeroVector;
        SealedActors.Empty();
        OnSealDeactivated();
    }
}

void AQliphothSeal::PlaceSeal(FVector Location)
{
    if (!bIsActive) return;

    SealLocation = Location;
    CheckSealedActors();
}

void AQliphothSeal::ApplySealEffects(AActor* Actor)
{
    if (!Actor) return;

    if (ACharacter* Character = Cast<ACharacter>(Actor))
    {
        float SealStrength = CalculateSealStrength(Actor);

        switch (CurrentSealType)
        {
            case ESealType::Containment:
                // Apply containment effects (immobilization and weakening)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.5f; // Significant speed reduction
                }
                break;

            case ESealType::Purification:
                // Apply purification effects (healing and cleansing)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f; // Slight speed boost
                }
                break;

            case ESealType::Transformation:
                // Apply transformation effects (balance and change)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f; // Moderate speed reduction
                }
                break;
        }
    }
}

float AQliphothSeal::CalculateSealStrength(AActor* Actor)
{
    if (!Actor) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(SealLocation, Actor->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / SealRadius), 0.0f, 1.0f);

    // Apply seal type modifier
    float TypeModifier = 1.0f;
    switch (CurrentSealType)
    {
        case ESealType::Containment:
            TypeModifier = 1.6f;
            break;
        case ESealType::Purification:
            TypeModifier = 1.4f;
            break;
        case ESealType::Transformation:
            TypeModifier = 1.5f;
            break;
    }

    return SealPower * DistanceFactor * TypeModifier;
}

void AQliphothSeal::ProcessSealedActors(float DeltaTime)
{
    for (AActor* Actor : SealedActors)
    {
        if (Actor && !Actor->IsPendingKill())
        {
            float SealStrength = CalculateSealStrength(Actor);
            float EffectAmount = SealStrength * DeltaTime;

            switch (CurrentSealType)
            {
                case ESealType::Containment:
                    // Apply containment effects over time
                    UGameplayStatics::ApplyDamage(Actor, EffectAmount * 0.3f, nullptr, this, nullptr);
                    break;

                case ESealType::Purification:
                    // Apply purification effects over time
                    UGameplayStatics::ApplyDamage(Actor, -EffectAmount * 0.5f, nullptr, this, nullptr);
                    break;

                case ESealType::Transformation:
                    // Apply transformation effects over time
                    UGameplayStatics::ApplyDamage(Actor, -EffectAmount * 0.2f, nullptr, this, nullptr);
                    break;
            }
        }
    }
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
}

void AQliphothSeal::CheckSealedActors()
{
    if (!bIsActive || SealLocation == FVector::ZeroVector) return;

    // Find all actors in range
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, SealLocation, SealRadius, ActorsInRange);

    // Update sealed actors list
    SealedActors.Empty();
    for (AActor* Actor : ActorsInRange)
    {
        if (Actor && !Actor->IsPendingKill())
        {
            SealedActors.Add(Actor);
            float SealStrength = CalculateSealStrength(Actor);
            ApplySealEffects(Actor);
            OnActorSealed(Actor, SealStrength);
        }
    }
} 