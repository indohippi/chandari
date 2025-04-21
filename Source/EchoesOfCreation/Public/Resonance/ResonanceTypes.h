#pragma once

#include "CoreMinimal.h"
#include "ResonanceTypes.generated.h"

UENUM(BlueprintType)
enum class EResonanceType : uint8
{
    Faith        UMETA(DisplayName = "Faith"),
    Doubt        UMETA(DisplayName = "Doubt"),
    Curiosity    UMETA(DisplayName = "Curiosity")
};

USTRUCT(BlueprintType)
struct FResonanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    EResonanceType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float Intensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float Duration;
};

USTRUCT(BlueprintType)
struct FResonanceEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float DistortionIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float ManifestationChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float CorruptionRate;
}; 