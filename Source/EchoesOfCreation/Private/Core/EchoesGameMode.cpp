#include "Core/EchoesGameMode.h"
#include "Resonance/ResonanceManager.h"
#include "Echoes/EchoManager.h"
#include "Characters/EchoesCharacter.h"
#include "Kismet/GameplayStatics.h"

AEchoesGameMode::AEchoesGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Set default values
    GameDuration = 300.0f; // 5 minutes
    ResonanceDecayRate = 0.1f;
    EchoSpawnRate = 1.0f;
}

void AEchoesGameMode::BeginPlay()
{
    Super::BeginPlay();
    InitializeGameSystems();
}

void AEchoesGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateGameState(DeltaTime);
}

void AEchoesGameMode::StartGame()
{
    if (ResonanceManager)
    {
        ResonanceManager->ResetResonance();
    }

    if (EchoManager)
    {
        EchoManager->ResetEchoes();
    }

    OnGameStarted();
}

void AEchoesGameMode::EndGame()
{
    // Calculate final scores and handle game end logic
    for (AEchoesPlayerState* PlayerState : RegisteredPlayers)
    {
        if (PlayerState)
        {
            // Handle player end game logic
        }
    }

    OnGameEnded();
}

void AEchoesGameMode::RegisterPlayer(AEchoesPlayerState* PlayerState)
{
    if (PlayerState && !RegisteredPlayers.Contains(PlayerState))
    {
        RegisteredPlayers.Add(PlayerState);
        OnPlayerRegistered(PlayerState);
    }
}

void AEchoesGameMode::UnregisterPlayer(AEchoesPlayerState* PlayerState)
{
    if (PlayerState)
    {
        RegisteredPlayers.Remove(PlayerState);
        OnPlayerUnregistered(PlayerState);
    }
}

void AEchoesGameMode::InitializeGameSystems()
{
    // Spawn Resonance Manager
    if (ResonanceManagerClass)
    {
        ResonanceManager = GetWorld()->SpawnActor<AResonanceManager>(ResonanceManagerClass);
    }

    // Spawn Echo Manager
    if (EchoManagerClass)
    {
        EchoManager = GetWorld()->SpawnActor<AEchoManager>(EchoManagerClass);
    }
}

void AEchoesGameMode::UpdateGameState(float DeltaTime)
{
    // Update game duration
    GameDuration -= DeltaTime;
    if (GameDuration <= 0.0f)
    {
        EndGame();
        return;
    }

    // Update game systems
    UpdateResonanceSystem(DeltaTime);
    UpdateEchoSystem(DeltaTime);
}

void AEchoesGameMode::UpdateResonanceSystem(float DeltaTime)
{
    if (ResonanceManager)
    {
        // Apply resonance decay
        TArray<EResonanceType> ResonanceTypes;
        ResonanceTypes.Add(EResonanceType::Faith);
        ResonanceTypes.Add(EResonanceType::Doubt);
        ResonanceTypes.Add(EResonanceType::Curiosity);

        for (EResonanceType Type : ResonanceTypes)
        {
            float CurrentResonance = ResonanceManager->GetResonance(Type);
            float NewResonance = FMath::Max(0.0f, CurrentResonance - (ResonanceDecayRate * DeltaTime));
            ResonanceManager->SetResonance(Type, NewResonance);
        }
    }
}

void AEchoesGameMode::UpdateEchoSystem(float DeltaTime)
{
    if (EchoManager)
    {
        // Handle echo spawning and management
        static float EchoSpawnTimer = 0.0f;
        EchoSpawnTimer += DeltaTime;

        if (EchoSpawnTimer >= EchoSpawnRate)
        {
            EchoSpawnTimer = 0.0f;
            // Spawn new echoes based on current resonance levels
            if (ResonanceManager)
            {
                float FaithResonance = ResonanceManager->GetResonance(EResonanceType::Faith);
                float DoubtResonance = ResonanceManager->GetResonance(EResonanceType::Doubt);
                float CuriosityResonance = ResonanceManager->GetResonance(EResonanceType::Curiosity);

                // Spawn echoes based on resonance levels
                if (FaithResonance > 0.5f)
                {
                    EchoManager->SpawnEcho(EEchoType::Divine);
                }
                if (DoubtResonance > 0.5f)
                {
                    EchoManager->SpawnEcho(EEchoType::Corrupted);
                }
                if (CuriosityResonance > 0.5f)
                {
                    EchoManager->SpawnEcho(EEchoType::Warped);
                }
            }
        }
    }
} 