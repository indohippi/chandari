#include "Abilities/ChakravartiAvatar/PranaConduit.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

APranaConduit::APranaConduit()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    ConnectedTarget = nullptr;
    ConduitRange = 1000.0f;
    PranaTransferRate = 1.0f;
}

void APranaConduit::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 1.8f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.2f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.0f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.2f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);
}

void APranaConduit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateConduitState(DeltaTime);
        if (ConnectedTarget)
        {
            if (!CheckTargetValidity(ConnectedTarget))
            {
                DisconnectFromTarget();
            }
            else
            {
                TransferPrana(ConnectedTarget, DeltaTime);
            }
        }
    }
}

void APranaConduit::InitializeConduit(EPranaType PranaType, float BasePower, float Duration)
{
    CurrentPranaType = PranaType;
    ConduitPower = BasePower;
    ConduitDuration = Duration;

    // Adjust properties based on prana type
    switch (PranaType)
    {
        case EPranaType::VitalFlow:
            ResonanceModifiers[EResonanceType::Faith] = 2.8f;
            EchoInteractions[EEchoType::Divine] = 2.8f;
            PranaTransferRate = 1.2f;
            break;
        case EPranaType::EnergySurge:
            ResonanceModifiers[EResonanceType::Doubt] = 2.2f;
            EchoInteractions[EEchoType::Corrupted] = 2.2f;
            PranaTransferRate = 1.5f;
            break;
        case EPranaType::LifeCycle:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.5f;
            EchoInteractions[EEchoType::Warped] = 2.5f;
            PranaTransferRate = 1.0f;
            break;
    }
}

void APranaConduit::ActivateConduit()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnConduitActivated();
    }
}

void APranaConduit::DeactivateConduit()
{
    if (bIsActive)
    {
        bIsActive = false;
        DisconnectFromTarget();
        OnConduitDeactivated();
    }
}

void APranaConduit::ConnectToTarget(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    ConnectedTarget = Target;
    float ConnectionStrength = CalculateConnectionStrength(Target);
    OnTargetConnected(Target, ConnectionStrength);
}

void APranaConduit::DisconnectFromTarget()
{
    if (ConnectedTarget)
    {
        AActor* OldTarget = ConnectedTarget;
        ConnectedTarget = nullptr;
        OnTargetDisconnected(OldTarget);
    }
}

void APranaConduit::ApplyConduitEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float ConnectionStrength = CalculateConnectionStrength(Target);

        switch (CurrentPranaType)
        {
            case EPranaType::VitalFlow:
                // Apply Vital Flow effects (healing and vitality)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.1f; // Speed boost
                }
                break;

            case EPranaType::EnergySurge:
                // Apply Energy Surge effects (power and energy)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.2f; // Greater speed boost
                }
                break;

            case EPranaType::LifeCycle:
                // Apply Life Cycle effects (balance and harmony)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 1.15f; // Moderate speed boost
                }
                break;
        }
    }
}

float APranaConduit::CalculateConnectionStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ConduitRange), 0.0f, 1.0f);

    // Apply prana type modifier
    float TypeModifier = 1.0f;
    switch (CurrentPranaType)
    {
        case EPranaType::VitalFlow:
            TypeModifier = 1.3f;
            break;
        case EPranaType::EnergySurge:
            TypeModifier = 1.5f;
            break;
        case EPranaType::LifeCycle:
            TypeModifier = 1.2f;
            break;
    }

    return ConduitPower * DistanceFactor * TypeModifier;
}

void APranaConduit::TransferPrana(AActor* Target, float DeltaTime)
{
    if (!Target) return;

    float ConnectionStrength = CalculateConnectionStrength(Target);
    float TransferAmount = ConnectionStrength * PranaTransferRate * DeltaTime;

    switch (CurrentPranaType)
    {
        case EPranaType::VitalFlow:
            // Transfer healing energy
            UGameplayStatics::ApplyDamage(Target, -TransferAmount, nullptr, this, nullptr);
            break;

        case EPranaType::EnergySurge:
            // Transfer power energy
            if (ACharacter* Character = Cast<ACharacter>(Target))
            {
                // Apply energy boost (to be implemented in character class)
            }
            break;

        case EPranaType::LifeCycle:
            // Transfer balanced energy
            UGameplayStatics::ApplyDamage(Target, -TransferAmount * 0.5f, nullptr, this, nullptr);
            if (ACharacter* Character = Cast<ACharacter>(Target))
            {
                // Apply balanced boost (to be implemented in character class)
            }
            break;
    }
}

void APranaConduit::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ConduitPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void APranaConduit::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ConduitPower);
        }
    }
}

void APranaConduit::UpdateConduitState(float DeltaTime)
{
    // Update conduit duration
    ConduitDuration -= DeltaTime;
    if (ConduitDuration <= 0.0f)
    {
        DeactivateConduit();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to connected target if valid
    if (ConnectedTarget && CheckTargetValidity(ConnectedTarget))
    {
        ApplyConduitEffects(ConnectedTarget);
    }
}

bool APranaConduit::CheckTargetValidity(AActor* Target)
{
    if (!Target || Target->IsPendingKill()) return false;

    // Check if target is within range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    return Distance <= ConduitRange;
} 