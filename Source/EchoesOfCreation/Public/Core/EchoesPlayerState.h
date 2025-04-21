#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Resonance/ResonanceTypes.h"
#include "Echoes/EchoTypes.h"
#include "EchoesPlayerState.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    CovenantWeaver UMETA(DisplayName = "Covenant Weaver"),
    PhilosopherKing UMETA(DisplayName = "Philosopher King"),
    ChakravartiAvatar UMETA(DisplayName = "Chakravarti Avatar"),
    SerpentsWhisper UMETA(DisplayName = "Serpent's Whisper"),
    JadeDragon UMETA(DisplayName = "Jade Dragon")
};

UCLASS()
class ECHOESOFCREATION_API AEchoesPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AEchoesPlayerState();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Character Class
    UFUNCTION(BlueprintCallable, Category = "Character")
    ECharacterClass GetCharacterClass() const { return CharacterClass; }

    UFUNCTION(BlueprintCallable, Category = "Character")
    void SetCharacterClass(ECharacterClass NewClass);

    // Resonance Management
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    float GetResonance(EResonanceType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void AddResonance(EResonanceType Type, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void SetResonance(EResonanceType Type, float Value);

    // Echo Interaction
    UFUNCTION(BlueprintCallable, Category = "Echoes")
    float GetEchoAffinity(EEchoType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Echoes")
    void ModifyEchoAffinity(EEchoType Type, float Modifier);

    // Score Management
    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetEchoesCollected() const { return EchoesCollected; }

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddEchoesCollected(int32 Amount);

protected:
    // Character Properties
    UPROPERTY(ReplicatedUsing = OnRep_CharacterClass)
    ECharacterClass CharacterClass;

    // Resonance Properties
    UPROPERTY(ReplicatedUsing = OnRep_Resonance)
    TMap<EResonanceType, float> ResonanceLevels;

    // Echo Properties
    UPROPERTY(ReplicatedUsing = OnRep_EchoAffinity)
    TMap<EEchoType, float> EchoAffinities;

    // Score Properties
    UPROPERTY(ReplicatedUsing = OnRep_Score)
    int32 EchoesCollected;

    // Replication Notifications
    UFUNCTION()
    void OnRep_CharacterClass();

    UFUNCTION()
    void OnRep_Resonance();

    UFUNCTION()
    void OnRep_EchoAffinity();

    UFUNCTION()
    void OnRep_Score();

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnCharacterClassChanged(ECharacterClass NewClass);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnResonanceChanged(EResonanceType Type, float NewValue);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnEchoAffinityChanged(EEchoType Type, float NewValue);

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnEchoesCollectedChanged(int32 NewCount);

private:
    void InitializePlayerState();
    void UpdateResonance(float DeltaTime);
}; 