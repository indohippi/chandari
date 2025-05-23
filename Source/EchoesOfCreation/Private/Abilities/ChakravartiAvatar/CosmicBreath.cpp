#include "Abilities/ChakravartiAvatar/CosmicBreath.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ACosmicBreath::ACosmicBreath()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and setup the collision sphere
    BreathSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BreathSphere"));
    RootComponent = BreathSphere;
    BreathSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    BreathSphere->SetGenerateOverlapEvents(true);

    bIsActive = false;
    BreathConeAngle = 45.0f;
    BreathRange = 1000.0f;
}

void ACosmicBreath::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    BreathRadius = 500.0f;
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

void ACosmicBreath::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateBreathState(DeltaTime);
        FindActorsInCone();
        CheckAffectedActors();
    }
}

void ACosmicBreath::InitializeBreath(ECosmicBreathType BreathType, float BasePower, float Duration)
{
    CurrentBreathType = BreathType;
    BreathPower = BasePower;
    BreathDuration = Duration;

    // Adjust properties based on breath type
    switch (BreathType)
    {
        case ECosmicBreathType::SolarFlare:
            BreathRadius = 400.0f;
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.5f;
            break;
        case ECosmicBreathType::LunarTide:
            BreathRadius = 600.0f;
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.0f;
            break;
        case ECosmicBreathType::StellarWind:
            BreathRadius = 300.0f;
            ResonanceModifiers[EResonanceType::Curiosity] = 2.8f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
            break;
    }

    BreathSphere->SetSphereRadius(BreathRadius);
}

void ACosmicBreath::ActivateBreath()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnBreathActivated();
        FindActorsInCone();
    }
}

void ACosmicBreath::DeactivateBreath()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnBreathDeactivated();
        AffectedActors.Empty();
    }
}

void ACosmicBreath::SetBreathCone(float Angle, float Range)
{
    BreathConeAngle = FMath::Clamp(Angle, 15.0f, 90.0f);
    BreathRange = FMath::Max(100.0f, Range);
}

void ACosmicBreath::ApplyBreathEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float EffectStrength = CalculateEffectStrength(Target);

        switch (CurrentBreathType)
        {
            case ECosmicBreathType::SolarFlare:
                // Apply Solar Flare effects (burning and purification)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f; // Slow down due to burning
                }
                // TODO: Apply burning damage over time
                break;

            case ECosmicBreathType::LunarTide:
                // Apply Lunar Tide effects (freezing and corruption)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.7f; // Slow down due to freezing
                }
                // TODO: Apply freezing effects
                break;

            case ECosmicBreathType::StellarWind:
                // Apply Stellar Wind effects (disruption and warping)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.9f; // Slight slowdown
                }
                // TODO: Apply warping effects
                break;
        }

        OnActorAffected(Target, EffectStrength);
    }
}

float ACosmicBreath::CalculateEffectStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / BreathRange), 0.0f, 1.0f);

    // Apply breath type modifier
    float TypeModifier = 1.0f;
    switch (CurrentBreathType)
    {
        case ECosmicBreathType::SolarFlare:
            TypeModifier = 1.2f;
            break;
        case ECosmicBreathType::LunarTide:
            TypeModifier = 1.5f;
            break;
        case ECosmicBreathType::StellarWind:
            TypeModifier = 1.3f;
            break;
    }

    return BreathPower * DistanceFactor * TypeModifier;
}

void ACosmicBreath::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = BreathPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void ACosmicBreath::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * BreathPower);
        }
    }
}

void ACosmicBreath::UpdateBreathState(float DeltaTime)
{
    // Update breath duration
    BreathDuration -= DeltaTime;
    if (BreathDuration <= 0.0f)
    {
        DeactivateBreath();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to all affected actors
    for (AActor* Actor : AffectedActors)
    {
        ApplyBreathEffects(Actor);
    }
}

void ACosmicBreath::CheckAffectedActors()
{
    // Remove invalid actors
    AffectedActors.RemoveAll([](AActor* Actor) {
        return !Actor || Actor->IsPendingKill();
    });

    // Check if actors are still in cone
    for (int32 i = AffectedActors.Num() - 1; i >= 0; --i)
    {
        if (!IsInBreathCone(AffectedActors[i]))
        {
            AffectedActors.RemoveAt(i);
        }
    }
}

void ACosmicBreath::FindActorsInCone()
{
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), BreathRange, ActorsInRange);

    for (AActor* Actor : ActorsInRange)
    {
        if (!AffectedActors.Contains(Actor) && IsInBreathCone(Actor))
        {
            AffectedActors.Add(Actor);
            float EffectStrength = CalculateEffectStrength(Actor);
            OnActorAffected(Actor, EffectStrength);
        }
    }
}

bool ACosmicBreath::IsInBreathCone(AActor* Target)
{
    if (!Target) return false;

    FVector DirectionToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector ForwardVector = GetActorForwardVector();

    float DotProduct = FVector::DotProduct(DirectionToTarget, ForwardVector);
    float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

    return Angle <= BreathConeAngle * 0.5f;
} 