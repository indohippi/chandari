#include "Core/EchoesGameState.h"
#include "Net/UnrealNetwork.h"

AEchoesGameState::AEchoesGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsGameActive = false;
    GameTimeRemaining = 300.0f; // 5 minutes default
}

void AEchoesGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEchoesGameState, bIsGameActive);
    DOREPLIFETIME(AEchoesGameState, GameTimeRemaining);
    DOREPLIFETIME(AEchoesGameState, GlobalResonance);
    DOREPLIFETIME(AEchoesGameState, EchoCounts);
}

void AEchoesGameState::BeginPlay()
{
    Super::BeginPlay();
    InitializeGameState();
}

void AEchoesGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateGameTime(DeltaTime);
}

float AEchoesGameState::GetGlobalResonance(EResonanceType Type) const
{
    const float* Value = GlobalResonance.Find(Type);
    return Value ? *Value : 0.0f;
}

void AEchoesGameState::SetGlobalResonance(EResonanceType Type, float Value)
{
    if (HasAuthority())
    {
        GlobalResonance.Add(Type, FMath::Clamp(Value, 0.0f, 1.0f));
        OnResonanceChanged(Type, Value);
    }
}

int32 AEchoesGameState::GetEchoCount(EEchoType Type) const
{
    const int32* Count = EchoCounts.Find(Type);
    return Count ? *Count : 0;
}

void AEchoesGameState::IncrementEchoCount(EEchoType Type)
{
    if (HasAuthority())
    {
        int32& Count = EchoCounts.FindOrAdd(Type);
        Count++;
        OnEchoCountChanged(Type, Count);
    }
}

void AEchoesGameState::DecrementEchoCount(EEchoType Type)
{
    if (HasAuthority())
    {
        int32& Count = EchoCounts.FindOrAdd(Type);
        Count = FMath::Max(0, Count - 1);
        OnEchoCountChanged(Type, Count);
    }
}

TArray<AEchoesPlayerState*> AEchoesGameState::GetPlayerStates() const
{
    TArray<AEchoesPlayerState*> PlayerStates;
    for (APlayerState* PS : PlayerArray)
    {
        if (AEchoesPlayerState* EchoesPS = Cast<AEchoesPlayerState>(PS))
        {
            PlayerStates.Add(EchoesPS);
        }
    }
    return PlayerStates;
}

void AEchoesGameState::OnRep_GameState()
{
    OnGameStateChanged(bIsGameActive);
}

void AEchoesGameState::OnRep_GameTime()
{
    OnGameTimeUpdated(GameTimeRemaining);
}

void AEchoesGameState::OnRep_Resonance()
{
    // Notify all resonance changes
    for (const auto& Pair : GlobalResonance)
    {
        OnResonanceChanged(Pair.Key, Pair.Value);
    }
}

void AEchoesGameState::OnRep_Echoes()
{
    // Notify all echo count changes
    for (const auto& Pair : EchoCounts)
    {
        OnEchoCountChanged(Pair.Key, Pair.Value);
    }
}

void AEchoesGameState::InitializeGameState()
{
    if (HasAuthority())
    {
        // Initialize resonance values
        GlobalResonance.Add(EResonanceType::Faith, 0.0f);
        GlobalResonance.Add(EResonanceType::Doubt, 0.0f);
        GlobalResonance.Add(EResonanceType::Curiosity, 0.0f);

        // Initialize echo counts
        EchoCounts.Add(EEchoType::Divine, 0);
        EchoCounts.Add(EEchoType::Corrupted, 0);
        EchoCounts.Add(EEchoType::Warped, 0);
    }
}

void AEchoesGameState::UpdateGameTime(float DeltaTime)
{
    if (HasAuthority() && bIsGameActive)
    {
        GameTimeRemaining = FMath::Max(0.0f, GameTimeRemaining - DeltaTime);
        if (GameTimeRemaining <= 0.0f)
        {
            bIsGameActive = false;
        }
    }
} 