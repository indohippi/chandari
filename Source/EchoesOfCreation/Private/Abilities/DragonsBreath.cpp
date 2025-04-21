#include "Abilities/DragonsBreath.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADragonsBreath::ADragonsBreath()
{
    // Set default values
    AbilityName = TEXT("Dragon's Breath");
    ResonanceCost = 0.3f;
    Cooldown = 15.0f;
    Range = 700.0f;
    SelectedType = EBreathType::Balance;
}

void ADragonsBreath::SelectBreathType(EBreathType Type)
{
    SelectedType = Type;
}

bool ADragonsBreath::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create breath effect
    CreateBreathEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for breath duration
    GetWorld()->GetTimerManager().SetTimer(
        BreathTimer,
        this,
        &ADragonsBreath::OnBreathComplete,
        BreathDuration,
        false
    );

    return true;
}

void ADragonsBreath::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the breath radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= BreathRadius)
        {
            ApplyBreathEffects(Character);
        }
    }
}

void ADragonsBreath::CreateBreathEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the dragon's breath
    // This could include particle effects, post-processing effects, etc.
}

void ADragonsBreath::ApplyBreathEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateBreathIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EBreathType::Yin:
            // Apply Yin effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.4f);
            break;

        case EBreathType::Yang:
            // Apply Yang effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.4f);
            break;

        case EBreathType::Balance:
            // Apply balanced effects
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.5f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.5f);
            break;

        case EBreathType::Chaos:
            // Apply chaotic effects
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.6f);
            break;

        case EBreathType::Dragon:
            // Apply draconic effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.7f);
            break;
    }
}

float ADragonsBreath::CalculateBreathIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / BreathRadius), 0.0f, 1.0f);
    
    return BreathIntensity * DistanceFactor;
}

void ADragonsBreath::OnBreathComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 