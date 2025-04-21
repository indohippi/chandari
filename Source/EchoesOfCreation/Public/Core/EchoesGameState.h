#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Resonance/ResonanceTypes.h"
#include "Echoes/EchoTypes.h"
#include "EchoesGameState.generated.h"

class AEchoesPlayerState;

UCLASS()
class ECHOESOFCREATION_API AEchoesGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AEchoesGameState();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Game State Management
    UFUNCTION(BlueprintCallable, Category = "Game State")
    bool IsGameActive() const { return bIsGameActive; }

    UFUNCTION(BlueprintCallable, Category = "Game State")
    float GetGameTimeRemaining() const { return GameTimeRemaining; }

    // Resonance State
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    float GetGlobalResonance(EResonanceType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void SetGlobalResonance(EResonanceType Type, float Value);

    // Echo State
    UFUNCTION(BlueprintCallable, Category = "Echoes")
    int32 GetEchoCount(EEchoType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Echoes")
    void IncrementEchoCount(EEchoType Type);

    UFUNCTION(BlueprintCallable, Category = "Echoes")
    void DecrementEchoCount(EEchoType Type);

    // Player Management
    UFUNCTION(BlueprintCallable, Category = "Players")
    TArray<AEchoesPlayerState*> GetPlayerStates() const;

protected:
    // Game State
    UPROPERTY(ReplicatedUsing = OnRep_GameState)
    bool bIsGameActive;

    UPROPERTY(ReplicatedUsing = OnRep_GameTime)
    float GameTimeRemaining;

    // Resonance State
    UPROPERTY(ReplicatedUsing = OnRep_Resonance)
    TMap<EResonanceType, float> GlobalResonance;

    // Echo State
    UPROPERTY(ReplicatedUsing = OnRep_Echoes)
    TMap<EEchoType, int32> EchoCounts;

    // Replication Notifications
    UFUNCTION()
    void OnRep_GameState();

    UFUNCTION()
    void OnRep_GameTime();

    UFUNCTION()
    void OnRep_Resonance();

    UFUNCTION()
    void OnRep_Echoes();

    // Game Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnGameStateChanged(bool bNewState);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnGameTimeUpdated(float NewTime);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnResonanceChanged(EResonanceType Type, float NewValue);

    UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
    void OnEchoCountChanged(EEchoType Type, int32 NewCount);

private:
    void InitializeGameState();
    void UpdateGameTime(float DeltaTime);
}; 