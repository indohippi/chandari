#include "Abilities/ChakravartiAvatar/PranaConduit.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

APranaConduit::APranaConduit()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    ConnectedTarget = nullptr;
    PranaTransferRate = 10.0f;
}

void APranaConduit::BeginPlay()
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

void APranaConduit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive && ConnectedTarget)
    {
        UpdateConduitState(DeltaTime);
        if (!CheckTargetValidity(ConnectedTarget))
        {
            DisconnectTarget();
        }
        else
        {
            TransferPrana(ConnectedTarget, DeltaTime);
        }
    }
}

void APranaConduit::InitializeConduit(float BasePower, float Duration)
{
    ConduitPower = BasePower;
    ConduitDuration = Duration;
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
        DisconnectTarget();
        OnConduitDeactivated();
    }
}

void APranaConduit::ConnectTarget(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    ConnectedTarget = Target;
    float ConnectionStrength = CalculateConnectionStrength(Target);
    OnTargetConnected(Target, ConnectionStrength);
}

void APranaConduit::DisconnectTarget()
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

        // Apply movement speed boost based on connection strength
        if (Character->GetCharacterMovement())
        {
            Character->GetCharacterMovement()->MaxWalkSpeed *= (1.0f + ConnectionStrength * 0.2f);
        }

        // Apply health regeneration based on connection strength
        if (Character->GetHealthComponent())
        {
            float HealAmount = PranaTransferRate * ConnectionStrength * GetWorld()->GetDeltaSeconds();
            Character->GetHealthComponent()->Heal(HealAmount);
        }
    }
}

float APranaConduit::CalculateConnectionStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ConduitRange), 0.0f, 1.0f);

    // Calculate alignment factor based on resonance types
    float AlignmentFactor = 1.0f;
    if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Target))
    {
        // Check resonance alignment between source and target
        for (const auto& Modifier : ResonanceModifiers)
        {
            float TargetResonance = EchoesChar->GetResonance(Modifier.Key);
            AlignmentFactor += TargetResonance * 0.1f;
        }
    }

    return ConduitPower * DistanceFactor * AlignmentFactor;
}

void APranaConduit::TransferPrana(AActor* Target, float DeltaTime)
{
    if (!Target) return;

    float ConnectionStrength = CalculateConnectionStrength(Target);
    float TransferAmount = PranaTransferRate * ConnectionStrength * DeltaTime;

    // Transfer health from source to target
    if (ACharacter* SourceCharacter = Cast<ACharacter>(GetOwner()))
    {
        if (ACharacter* TargetCharacter = Cast<ACharacter>(Target))
        {
            if (SourceCharacter->GetHealthComponent() && TargetCharacter->GetHealthComponent())
            {
                // Source loses health
                SourceCharacter->GetHealthComponent()->TakeDamage(TransferAmount, FDamageEvent(), nullptr, this);
                // Target gains health
                TargetCharacter->GetHealthComponent()->Heal(TransferAmount);
            }
        }
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

    // Apply effects to connected target
    if (ConnectedTarget)
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