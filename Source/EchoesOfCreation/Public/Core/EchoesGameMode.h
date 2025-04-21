#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Resonance/ResonanceTypes.h"
#include "Echoes/EchoTypes.h"
#include "EchoesGameMode.generated.h"

class AResonanceManager;
class AEchoManager;
class AEchoesPlayerState;

UCLASS()
class ECHOESOFCREATION_API AEchoesGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AEchoesGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Game State Management
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void EndGame();

    // Resonance and Echo Management
    UFUNCTION(BlueprintCallable, Category = "Game Systems")
    AResonanceManager* GetResonanceManager() const { return ResonanceManager; }

    UFUNCTION(BlueprintCallable, Category = "Game Systems")
    AEchoManager* GetEchoManager() const { return EchoManager; }

    // Player Management
    UFUNCTION(BlueprintCallable, Category = "Player Management")
    void RegisterPlayer(AEchoesPlayerState* PlayerState);

    UFUNCTION(BlueprintCallable, Category = "Player Management")
    void UnregisterPlayer(AEchoesPlayerState* PlayerState);

protected:
    // Game Systems
    UPROPERTY(EditDefaultsOnly, Category = "Game Systems")
    TSubclassOf<AResonanceManager> ResonanceManagerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game Systems")
    TSubclassOf<AEchoManager> EchoManagerClass;

    UPROPERTY()
    AResonanceManager* ResonanceManager;

    UPROPERTY()
    AEchoManager* EchoManager;

    // Game State
    UPROPERTY(EditDefaultsOnly, Category = "Game State")
    float GameDuration;

    UPROPERTY(EditDefaultsOnly, Category = "Game State")
    float ResonanceDecayRate;

    UPROPERTY(EditDefaultsOnly, Category = "Game State")
    float EchoSpawnRate;

    UPROPERTY()
    TArray<AEchoesPlayerState*> RegisteredPlayers;

    // Game Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnGameStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnGameEnded();

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnPlayerRegistered(AEchoesPlayerState* PlayerState);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnPlayerUnregistered(AEchoesPlayerState* PlayerState);

private:
    void InitializeGameSystems();
    void UpdateGameState(float DeltaTime);
    void UpdateResonanceSystem(float DeltaTime);
    void UpdateEchoSystem(float DeltaTime);
}; 