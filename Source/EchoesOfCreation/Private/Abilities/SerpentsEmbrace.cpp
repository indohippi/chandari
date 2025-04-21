#include "Abilities/SerpentsEmbrace.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ASerpentsEmbrace::ASerpentsEmbrace()
{
    // Set default values
    AbilityName = TEXT("Serpent's Embrace");
    ResonanceCost = 0.45f;
    Cooldown = 25.0f;
    Range = 600.0f;
    SelectedType = EEmbraceType::Binding;
}

void ASerpentsEmbrace::SelectEmbraceType(EEmbraceType Type)
{
    SelectedType = Type;
}

bool ASerpentsEmbrace::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create embrace effect
    CreateEmbraceEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for embrace duration
    GetWorld()->GetTimerManager().SetTimer(
        EmbraceTimer,
        this,
        &ASerpentsEmbrace::OnEmbraceComplete,
        EmbraceDuration,
        false
    );

    return true;
}

void ASerpentsEmbrace::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the embrace radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= EmbraceRadius)
        {
            ApplyEmbraceEffects(Character);
        }
    }
}

void ASerpentsEmbrace::CreateEmbraceEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the serpent's embrace
    // This could include particle effects, post-processing effects, etc.
}

void ASerpentsEmbrace::ApplyEmbraceEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateEmbraceIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EEmbraceType::Binding:
            // Apply binding effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.5f);
            break;

        case EEmbraceType::Knowledge:
            // Apply knowledge effects
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.6f);
            break;

        case EEmbraceType::Transformation:
            // Apply transformation effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.7f);
            break;

        case EEmbraceType::Corruption:
            // Apply corruption effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.9f);
            EchoComp->ApplyEchoEffect(EEchoType::CorruptionSpread, Intensity * 0.8f);
            break;

        case EEmbraceType::Ascension:
            // Apply ascension effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.9f);
            break;
    }
}

float ASerpentsEmbrace::CalculateEmbraceIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / EmbraceRadius), 0.0f, 1.0f);
    
    return EmbraceIntensity * DistanceFactor;
}

void ASerpentsEmbrace::OnEmbraceComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 