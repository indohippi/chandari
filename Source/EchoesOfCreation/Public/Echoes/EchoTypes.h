#pragma once

#include "CoreMinimal.h"
#include "EchoTypes.generated.h"

UENUM(BlueprintType)
enum class EEchoType : uint8
{
    Benevolent    UMETA(DisplayName = "Benevolent"),
    Corrupted     UMETA(DisplayName = "Corrupted"),
    Neutral       UMETA(DisplayName = "Neutral")
};

UENUM(BlueprintType)
enum class EEchoManifestation : uint8
{
    Visual        UMETA(DisplayName = "Visual"),
    Auditory      UMETA(DisplayName = "Auditory"),
    Physical      UMETA(DisplayName = "Physical"),
    Mental        UMETA(DisplayName = "Mental")
};

USTRUCT(BlueprintType)
struct FEchoData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    EEchoType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float Power;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    TArray<EEchoManifestation> Manifestations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float InfluenceRadius;
};

USTRUCT(BlueprintType)
struct FEchoEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float RealityWarping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float KnowledgeTransfer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float CorruptionSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    float HarmonyRestoration;
}; 