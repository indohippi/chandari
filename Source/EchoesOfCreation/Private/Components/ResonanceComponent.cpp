#include "Components/ResonanceComponent.h"
#include "Resonance/ResonanceManager.h"
#include "TimerManager.h"
#include "Engine/World.h"

UResonanceComponent::UResonanceComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Default values
    ResonanceResistance = 1.0f;
    ResonanceSensitivity = 1.0f;
}

void UResonanceComponent::BeginPlay()
{
    Super::BeginPlay();

    // Start the resonance update timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ResonanceUpdateTimer,
            this,
            &UResonanceComponent::UpdateResonanceEffects,
            0.1f, // Update every 0.1 seconds
            true
        );
    }
}

void UResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UResonanceComponent::GenerateResonance(EResonanceType Type, float Intensity, float Duration)
{
    if (UWorld* World = GetWorld())
    {
        if (UResonanceManager* ResonanceManager = World->GetGameInstance()->GetSubsystem<UResonanceManager>())
        {
            // Create resonance data
            FResonanceData ResonanceData;
            ResonanceData.Type = Type;
            ResonanceData.Intensity = Intensity * ResonanceSensitivity;
            ResonanceData.Location = GetOwner()->GetActorLocation();
            ResonanceData.Duration = Duration;

            // Generate resonance through manager
            ResonanceManager->GenerateResonance(ResonanceData);

            // Broadcast the event
            OnResonanceGenerated.Broadcast(ResonanceData);
        }
    }
}

void UResonanceComponent::UpdateResonanceEffects(float DeltaTime)
{
    if (UWorld* World = GetWorld())
    {
        if (UResonanceManager* ResonanceManager = World->GetGameInstance()->GetSubsystem<UResonanceManager>())
        {
            // Create resonance data for current position
            FResonanceData CurrentResonance;
            CurrentResonance.Location = GetOwner()->GetActorLocation();
            CurrentResonance.Intensity = 1.0f; // Base intensity for effect calculation

            // Get resonance effect for current position
            FResonanceEffect Effect = ResonanceManager->CalculateResonanceEffect(CurrentResonance);

            // Apply resistance
            Effect.DistortionIntensity *= (1.0f / ResonanceResistance);
            Effect.ManifestationChance *= (1.0f / ResonanceResistance);
            Effect.CorruptionRate *= (1.0f / ResonanceResistance);

            // Update current effect
            CurrentResonanceEffect = Effect;

            // Handle the effect
            HandleResonanceEffect(Effect);
        }
    }
}

void UResonanceComponent::HandleResonanceEffect(const FResonanceEffect& Effect)
{
    // Broadcast the effect
    OnResonanceEffectReceived.Broadcast(Effect);

    // Apply visual effects based on distortion intensity
    if (Effect.DistortionIntensity > 0.5f)
    {
        // TODO: Apply visual distortion effects
    }

    // Check for manifestation
    if (FMath::RandRange(0.0f, 1.0f) < Effect.ManifestationChance)
    {
        // TODO: Trigger echo manifestation
    }

    // Apply corruption if needed
    if (Effect.CorruptionRate > 0.0f)
    {
        // TODO: Apply corruption effects
    }
} 