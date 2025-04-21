#include "Abilities/DharmaShield.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ADharmaShield::ADharmaShield()
{
    // Set default values
    AbilityName = TEXT("Dharma Shield");
    ResonanceCost = 0.5f;
    Cooldown = 25.0f;
    Range = 1000.0f;
    SelectedType = EDharmaType::Protection;
}

void ADharmaShield::SelectDharmaType(EDharmaType Type)
{
    SelectedType = Type;
}

bool ADharmaShield::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create Dharma shield effect
    CreateShieldEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for shield duration
    GetWorld()->GetTimerManager().SetTimer(
        ShieldTimer,
        this,
        &ADharmaShield::OnShieldComplete,
        ShieldDuration,
        false
    );

    return true;
}

void ADharmaShield::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the shield radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= ShieldRadius)
        {
            ApplyDharmaEffects(Character);
        }
    }
}

void ADharmaShield::CreateShieldEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the Dharma shield
    // This could include particle effects, post-processing effects, etc.
}

void ADharmaShield::ApplyDharmaEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateShieldIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EDharmaType::Protection:
            // Apply protective Dharma effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.7f);
            break;

        case EDharmaType::Reflection:
            // Apply reflective Dharma effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.6f);
            break;

        case EDharmaType::Absorption:
            // Apply absorptive Dharma effects
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.7f);
            break;

        case EDharmaType::Purification:
            // Apply purifying Dharma effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.9f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.8f);
            break;

        case EDharmaType::Enlightenment:
            // Apply enlightened Dharma effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.9f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.7f);
            break;
    }
}

float ADharmaShield::CalculateShieldIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ShieldRadius), 0.0f, 1.0f);
    
    return ShieldStrength * DistanceFactor;
}

void ADharmaShield::OnShieldComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 
} 