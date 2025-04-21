#include "Resonance/ResonanceManager.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UResonanceManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Start the resonance update timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ResonanceUpdateTimer,
            this,
            &UResonanceManager::UpdateResonanceEffects,
            0.1f, // Update every 0.1 seconds
            true
        );
    }
}

void UResonanceManager::Deinitialize()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ResonanceUpdateTimer);
    }

    Super::Deinitialize();
}

void UResonanceManager::GenerateResonance(const FResonanceData& ResonanceData)
{
    ActiveResonances.Add(ResonanceData);
    OnResonanceChanged.Broadcast(ResonanceData);
}

FResonanceEffect UResonanceManager::CalculateResonanceEffect(const FResonanceData& ResonanceData) const
{
    FResonanceEffect Effect;
    
    // Base calculations for resonance effects
    Effect.DistortionIntensity = ResonanceData.Intensity * 0.5f;
    Effect.ManifestationChance = ResonanceData.Intensity * 0.3f;
    Effect.CorruptionRate = ResonanceData.Intensity * 0.2f;

    // Modify effects based on resonance type
    switch (ResonanceData.Type)
    {
        case EResonanceType::Faith:
            Effect.DistortionIntensity *= 0.8f;
            Effect.ManifestationChance *= 1.2f;
            Effect.CorruptionRate *= 0.5f;
            break;
        case EResonanceType::Doubt:
            Effect.DistortionIntensity *= 1.2f;
            Effect.ManifestationChance *= 0.8f;
            Effect.CorruptionRate *= 1.5f;
            break;
        case EResonanceType::Curiosity:
            Effect.DistortionIntensity *= 1.0f;
            Effect.ManifestationChance *= 1.5f;
            Effect.CorruptionRate *= 1.0f;
            break;
    }

    return Effect;
}

void UResonanceManager::UpdateResonanceEffects(float DeltaTime)
{
    // Update active resonances
    for (int32 i = ActiveResonances.Num() - 1; i >= 0; --i)
    {
        FResonanceData& Resonance = ActiveResonances[i];
        Resonance.Duration -= DeltaTime;

        if (Resonance.Duration <= 0.0f)
        {
            ActiveResonances.RemoveAt(i);
        }
    }
} 