#include "Components/EchoComponent.h"
#include "Echoes/EchoManager.h"
#include "TimerManager.h"
#include "Engine/World.h"

UEchoComponent::UEchoComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // Default values
    EchoDetectionRange = 1000.0f;
    EchoInteractionCooldown = 1.0f;
    LastInteractionTime = 0.0f;
}

void UEchoComponent::BeginPlay()
{
    Super::BeginPlay();

    // Start the echo detection timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            EchoDetectionTimer,
            this,
            &UEchoComponent::DetectEchoes,
            0.5f, // Check for echoes every 0.5 seconds
            true
        );
    }
}

void UEchoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEchoComponent::DetectEchoes(float Range)
{
    if (UWorld* World = GetWorld())
    {
        if (UEchoManager* EchoManager = World->GetGameInstance()->GetSubsystem<UEchoManager>())
        {
            // Get echoes in range
            TArray<FEchoData> EchoesInRange;
            EchoManager->GetEchoesInRange(GetOwner()->GetActorLocation(), Range, EchoesInRange);

            // Process each echo
            for (const FEchoData& Echo : EchoesInRange)
            {
                // Broadcast detection event
                OnEchoDetected.Broadcast(Echo);

                // Calculate and apply effects
                FEchoEffect Effect = EchoManager->CalculateEchoEffect(Echo);
                HandleEchoEffect(Effect);
            }
        }
    }
}

void UEchoComponent::InteractWithEcho(const FEchoData& EchoData)
{
    if (UWorld* World = GetWorld())
    {
        // Check cooldown
        float CurrentTime = World->GetTimeSeconds();
        if (CurrentTime - LastInteractionTime < EchoInteractionCooldown)
        {
            return;
        }

        if (UEchoManager* EchoManager = World->GetGameInstance()->GetSubsystem<UEchoManager>())
        {
            // Calculate interaction effect
            FEchoEffect Effect = EchoManager->CalculateEchoEffect(EchoData);
            
            // Apply the effect
            HandleEchoEffect(Effect);

            // Update last interaction time
            LastInteractionTime = CurrentTime;
        }
    }
}

void UEchoComponent::HandleEchoEffect(const FEchoEffect& Effect)
{
    // Update current effect
    CurrentEchoEffect = Effect;

    // Broadcast the effect
    OnEchoEffectApplied.Broadcast(Effect);

    // Apply reality warping effects
    if (Effect.RealityWarping > 0.0f)
    {
        // TODO: Apply visual and physical reality warping effects
    }

    // Process knowledge transfer
    if (Effect.KnowledgeTransfer > 0.0f)
    {
        // TODO: Apply knowledge transfer effects
    }

    // Handle corruption spread
    if (Effect.CorruptionSpread > 0.0f)
    {
        // TODO: Apply corruption effects
    }

    // Apply harmony restoration
    if (Effect.HarmonyRestoration > 0.0f)
    {
        // TODO: Apply harmony restoration effects
    }
} 