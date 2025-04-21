#include "Core/EchoesPlayerState.h"
#include "Net/UnrealNetwork.h"

AEchoesPlayerState::AEchoesPlayerState()
{
    PrimaryActorTick.bCanEverTick = true;
    CharacterClass = ECharacterClass::CovenantWeaver;
    EchoesCollected = 0;
}

void AEchoesPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEchoesPlayerState, CharacterClass);
    DOREPLIFETIME(AEchoesPlayerState, ResonanceLevels);
    DOREPLIFETIME(AEchoesPlayerState, EchoAffinities);
    DOREPLIFETIME(AEchoesPlayerState, EchoesCollected);
}

void AEchoesPlayerState::BeginPlay()
{
    Super::BeginPlay();
    InitializePlayerState();
}

void AEchoesPlayerState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateResonance(DeltaTime);
}

void AEchoesPlayerState::SetCharacterClass(ECharacterClass NewClass)
{
    if (CharacterClass != NewClass)
    {
        CharacterClass = NewClass;
        OnCharacterClassChanged(NewClass);
    }
}

float AEchoesPlayerState::GetResonance(EResonanceType Type) const
{
    return ResonanceLevels.Contains(Type) ? ResonanceLevels[Type] : 0.0f;
}

void AEchoesPlayerState::AddResonance(EResonanceType Type, float Amount)
{
    float CurrentValue = GetResonance(Type);
    SetResonance(Type, CurrentValue + Amount);
}

void AEchoesPlayerState::SetResonance(EResonanceType Type, float Value)
{
    Value = FMath::Clamp(Value, 0.0f, 100.0f);
    ResonanceLevels.Add(Type, Value);
    OnResonanceChanged(Type, Value);
}

float AEchoesPlayerState::GetEchoAffinity(EEchoType Type) const
{
    return EchoAffinities.Contains(Type) ? EchoAffinities[Type] : 1.0f;
}

void AEchoesPlayerState::ModifyEchoAffinity(EEchoType Type, float Modifier)
{
    float CurrentValue = GetEchoAffinity(Type);
    float NewValue = CurrentValue * Modifier;
    EchoAffinities.Add(Type, NewValue);
    OnEchoAffinityChanged(Type, NewValue);
}

void AEchoesPlayerState::AddEchoesCollected(int32 Amount)
{
    EchoesCollected += Amount;
    OnEchoesCollectedChanged(EchoesCollected);
}

void AEchoesPlayerState::OnRep_CharacterClass()
{
    OnCharacterClassChanged(CharacterClass);
}

void AEchoesPlayerState::OnRep_Resonance()
{
    for (const auto& Pair : ResonanceLevels)
    {
        OnResonanceChanged(Pair.Key, Pair.Value);
    }
}

void AEchoesPlayerState::OnRep_EchoAffinity()
{
    for (const auto& Pair : EchoAffinities)
    {
        OnEchoAffinityChanged(Pair.Key, Pair.Value);
    }
}

void AEchoesPlayerState::OnRep_Score()
{
    OnEchoesCollectedChanged(EchoesCollected);
}

void AEchoesPlayerState::InitializePlayerState()
{
    // Initialize resonance levels
    SetResonance(EResonanceType::Faith, 0.0f);
    SetResonance(EResonanceType::Doubt, 0.0f);
    SetResonance(EResonanceType::Curiosity, 0.0f);

    // Initialize echo affinities
    ModifyEchoAffinity(EEchoType::Divine, 1.0f);
    ModifyEchoAffinity(EEchoType::Corrupted, 1.0f);
    ModifyEchoAffinity(EEchoType::Warped, 1.0f);
}

void AEchoesPlayerState::UpdateResonance(float DeltaTime)
{
    // Apply natural decay to resonance levels
    for (auto& Pair : ResonanceLevels)
    {
        float DecayRate = 0.1f; // Adjust this value as needed
        float NewValue = FMath::Max(0.0f, Pair.Value - (DecayRate * DeltaTime));
        SetResonance(Pair.Key, NewValue);
    }
} 