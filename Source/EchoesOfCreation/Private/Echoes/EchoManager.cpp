#include "Echoes/EchoManager.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Resonance/ResonanceManager.h"

void UEchoManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Start the echo update timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            EchoUpdateTimer,
            this,
            &UEchoManager::UpdateEchoEffects,
            0.1f, // Update every 0.1 seconds
            true
        );
    }
}

void UEchoManager::Deinitialize()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(EchoUpdateTimer);
    }

    Super::Deinitialize();
}

void UEchoManager::ProcessResonance(const FResonanceData& ResonanceData)
{
    CheckEchoManifestation(ResonanceData);
}

FEchoEffect UEchoManager::CalculateEchoEffect(const FEchoData& EchoData) const
{
    FEchoEffect Effect;
    
    // Base calculations for echo effects
    Effect.RealityWarping = EchoData.Power * 0.3f;
    Effect.KnowledgeTransfer = EchoData.Power * 0.2f;
    Effect.CorruptionSpread = EchoData.Power * 0.1f;
    Effect.HarmonyRestoration = EchoData.Power * 0.1f;

    // Modify effects based on echo type
    switch (EchoData.Type)
    {
        case EEchoType::Benevolent:
            Effect.RealityWarping *= 0.5f;
            Effect.KnowledgeTransfer *= 1.5f;
            Effect.CorruptionSpread *= 0.2f;
            Effect.HarmonyRestoration *= 2.0f;
            break;
        case EEchoType::Corrupted:
            Effect.RealityWarping *= 2.0f;
            Effect.KnowledgeTransfer *= 0.5f;
            Effect.CorruptionSpread *= 2.0f;
            Effect.HarmonyRestoration *= 0.2f;
            break;
        case EEchoType::Neutral:
            Effect.RealityWarping *= 1.0f;
            Effect.KnowledgeTransfer *= 1.0f;
            Effect.CorruptionSpread *= 1.0f;
            Effect.HarmonyRestoration *= 1.0f;
            break;
    }

    return Effect;
}

void UEchoManager::RegisterEcho(const FEchoData& EchoData)
{
    KnownEchoes.Add(EchoData.Name, EchoData);
}

void UEchoManager::UpdateEchoEffects(float DeltaTime)
{
    // Update active echoes
    for (int32 i = ActiveEchoes.Num() - 1; i >= 0; --i)
    {
        FEchoData& Echo = ActiveEchoes[i];
        ApplyEchoEffects(Echo);
    }
}

void UEchoManager::CheckEchoManifestation(const FResonanceData& ResonanceData)
{
    // Check if resonance triggers any known echoes
    for (const auto& EchoPair : KnownEchoes)
    {
        const FEchoData& Echo = EchoPair.Value;
        
        // Check if echo is within influence radius
        float Distance = FVector::Distance(ResonanceData.Location, Echo.Location);
        if (Distance <= Echo.InfluenceRadius)
        {
            // Calculate manifestation chance based on resonance type and intensity
            float ManifestationChance = ResonanceData.Intensity * 0.5f;
            
            // Modify chance based on echo type
            switch (Echo.Type)
            {
                case EEchoType::Benevolent:
                    if (ResonanceData.Type == EResonanceType::Faith)
                        ManifestationChance *= 1.5f;
                    break;
                case EEchoType::Corrupted:
                    if (ResonanceData.Type == EResonanceType::Doubt)
                        ManifestationChance *= 1.5f;
                    break;
                case EEchoType::Neutral:
                    if (ResonanceData.Type == EResonanceType::Curiosity)
                        ManifestationChance *= 1.5f;
                    break;
            }

            // Random chance to manifest
            if (FMath::RandRange(0.0f, 1.0f) < ManifestationChance)
            {
                ActiveEchoes.Add(Echo);
                OnEchoManifested.Broadcast(Echo);
            }
        }
    }
}

void UEchoManager::ApplyEchoEffects(const FEchoData& EchoData)
{
    FEchoEffect Effect = CalculateEchoEffect(EchoData);
    OnEchoEffectApplied.Broadcast(Effect);
} 