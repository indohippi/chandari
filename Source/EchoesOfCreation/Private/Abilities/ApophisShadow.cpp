#include "Abilities/ApophisShadow.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AApophisShadow::AApophisShadow()
{
    // Set default values
    AbilityName = TEXT("Apophis' Shadow");
    ResonanceCost = 0.4f;
    Cooldown = 20.0f;
    Range = 1000.0f;
    SelectedType = EShadowType::Chaos;
}

void AApophisShadow::SelectShadowType(EShadowType Type)
{
    SelectedType = Type;
}

bool AApophisShadow::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create shadow effect
    CreateShadowEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for shadow duration
    GetWorld()->GetTimerManager().SetTimer(
        ShadowTimer,
        this,
        &AApophisShadow::OnShadowComplete,
        ShadowDuration,
        false
    );

    return true;
}

void AApophisShadow::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the shadow radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= ShadowRadius)
        {
            ApplyShadowEffects(Character);
        }
    }
}

void AApophisShadow::CreateShadowEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the shadow
    // This could include particle effects, post-processing effects, etc.
}

void AApophisShadow::ApplyShadowEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateShadowIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EShadowType::Chaos:
            // Apply chaos effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.6f);
            break;

        case EShadowType::Darkness:
            // Apply darkness effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, -Intensity * 0.4f);
            break;

        case EShadowType::Corruption:
            // Apply corruption effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::CorruptionSpread, Intensity * 0.5f);
            break;

        case EShadowType::Void:
            // Apply void effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.9f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.8f);
            break;

        case EShadowType::Serpent:
            // Apply Apophis' essence
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 1.0f);
            EchoComp->ApplyEchoEffect(EEchoType::CorruptionSpread, Intensity * 0.7f);
            break;
    }
}

float AApophisShadow::CalculateShadowIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ShadowRadius), 0.0f, 1.0f);
    
    return ChaosIntensity * DistanceFactor;
}

void AApophisShadow::OnShadowComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 