#include "Abilities/QiConduit.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AQiConduit::AQiConduit()
{
    // Set default values
    AbilityName = TEXT("Qi Conduit");
    ResonanceCost = 0.4f;
    Cooldown = 22.0f;
    Range = 900.0f;
    SelectedType = EQiType::Flow;
}

void AQiConduit::SelectQiType(EQiType Type)
{
    SelectedType = Type;
}

bool AQiConduit::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create Qi conduit effect
    CreateConduitEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for conduit duration
    GetWorld()->GetTimerManager().SetTimer(
        ConduitTimer,
        this,
        &AQiConduit::OnConduitComplete,
        ConduitDuration,
        false
    );

    return true;
}

void AQiConduit::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the conduit radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= ConduitRadius)
        {
            ApplyQiEffects(Character);
        }
    }
}

void AQiConduit::CreateConduitEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the Qi conduit
    // This could include particle effects, post-processing effects, etc.
}

void AQiConduit::ApplyQiEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateQiIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EQiType::Flow:
            // Apply flowing Qi effects
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.5f);
            break;

        case EQiType::Focus:
            // Apply focused Qi effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.6f);
            break;

        case EQiType::Balance:
            // Apply balanced Qi effects
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.7f);
            break;

        case EQiType::Disrupt:
            // Apply disruptive Qi effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.6f);
            break;

        case EQiType::Dragon:
            // Apply draconic Qi effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.9f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.7f);
            break;
    }
}

float AQiConduit::CalculateQiIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ConduitRadius), 0.0f, 1.0f);
    
    return QiIntensity * DistanceFactor;
}

void AQiConduit::OnConduitComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 