#pragma once

#include "CoreMinimal.h"
#include "ResonanceTypes.h"
#include "EchoTypes.generated.h"

UENUM(BlueprintType)
enum class EEchoType : uint8
{
    // Primary Echoes
    CreationEcho    UMETA(DisplayName = "Creation Echo"),
    DestructionEcho UMETA(DisplayName = "Destruction Echo"),
    BalanceEcho     UMETA(DisplayName = "Balance Echo"),
    
    // Elemental Echoes
    FireEcho        UMETA(DisplayName = "Fire Echo"),
    WaterEcho       UMETA(DisplayName = "Water Echo"),
    EarthEcho       UMETA(DisplayName = "Earth Echo"),
    AirEcho         UMETA(DisplayName = "Air Echo"),
    AetherEcho      UMETA(DisplayName = "Aether Echo"),
    
    // Spiritual Echoes
    LightEcho       UMETA(DisplayName = "Light Echo"),
    ShadowEcho      UMETA(DisplayName = "Shadow Echo"),
    LifeEcho        UMETA(DisplayName = "Life Echo"),
    DeathEcho       UMETA(DisplayName = "Death Echo"),
    
    // Meta Echoes
    TimeEcho        UMETA(DisplayName = "Time Echo"),
    SpaceEcho       UMETA(DisplayName = "Space Echo"),
    MindEcho        UMETA(DisplayName = "Mind Echo"),
    SoulEcho        UMETA(DisplayName = "Soul Echo")
};

USTRUCT(BlueprintType)
struct FEchoEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    EEchoType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float Intensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    FResonanceContainer ResonanceModifiers;

    FEchoEffect()
        : Type(EEchoType::BalanceEcho)
        , Intensity(1.0f)
        , Duration(0.0f)
    {}

    FEchoEffect(EEchoType InType, float InIntensity = 1.0f, float InDuration = 0.0f)
        : Type(InType)
        , Intensity(InIntensity)
        , Duration(InDuration)
    {}
};

USTRUCT(BlueprintType)
struct FEchoContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    TArray<FEchoEffect> ActiveEchoes;

    // Helper functions for managing echoes
    void AddEcho(const FEchoEffect& Echo);
    void RemoveEcho(EEchoType Type);
    bool HasEcho(EEchoType Type) const;
    void UpdateEchoes(float DeltaTime);
    void ClearEchoes();
}; 