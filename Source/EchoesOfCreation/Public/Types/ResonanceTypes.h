#pragma once

#include "CoreMinimal.h"
#include "ResonanceTypes.generated.h"

UENUM(BlueprintType)
enum class EResonanceType : uint8
{
    // Primary Resonances
    Creation     UMETA(DisplayName = "Creation Resonance"),
    Destruction  UMETA(DisplayName = "Destruction Resonance"),
    Balance      UMETA(DisplayName = "Balance Resonance"),
    
    // Elemental Resonances
    Fire        UMETA(DisplayName = "Fire Resonance"),
    Water       UMETA(DisplayName = "Water Resonance"),
    Earth       UMETA(DisplayName = "Earth Resonance"),
    Air         UMETA(DisplayName = "Air Resonance"),
    Aether      UMETA(DisplayName = "Aether Resonance"),
    
    // Spiritual Resonances
    Light       UMETA(DisplayName = "Light Resonance"),
    Shadow      UMETA(DisplayName = "Shadow Resonance"),
    Life        UMETA(DisplayName = "Life Resonance"),
    Death       UMETA(DisplayName = "Death Resonance"),
    
    // Meta Resonances
    Time        UMETA(DisplayName = "Time Resonance"),
    Space       UMETA(DisplayName = "Space Resonance"),
    Mind        UMETA(DisplayName = "Mind Resonance"),
    Soul        UMETA(DisplayName = "Soul Resonance")
};

USTRUCT(BlueprintType)
struct FResonanceValue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    EResonanceType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float MaxValue;

    FResonanceValue()
        : Type(EResonanceType::Balance)
        , Value(0.0f)
        , MaxValue(100.0f)
    {}

    FResonanceValue(EResonanceType InType, float InValue = 0.0f, float InMaxValue = 100.0f)
        : Type(InType)
        , Value(InValue)
        , MaxValue(InMaxValue)
    {}
};

USTRUCT(BlueprintType)
struct FResonanceContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    TMap<EResonanceType, FResonanceValue> Resonances;

    // Helper functions for managing resonances
    void AddResonance(EResonanceType Type, float Amount);
    void RemoveResonance(EResonanceType Type, float Amount);
    float GetResonanceValue(EResonanceType Type) const;
    bool HasResonance(EResonanceType Type) const;
    void ClearResonances();
}; 