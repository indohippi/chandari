#include "Abilities/ScalesOfMaAt.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AScalesOfMaAt::AScalesOfMaAt()
{
    // Set default values
    AbilityName = TEXT("The Scales of Ma'at");
    ResonanceCost = 0.35f;
    Cooldown = 18.0f;
    Range = 800.0f;
    SelectedType = EBalanceType::Justice;
}

void AScalesOfMaAt::SelectBalanceType(EBalanceType Type)
{
    SelectedType = Type;
}

bool AScalesOfMaAt::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create balance effect
    CreateBalanceEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for balance duration
    GetWorld()->GetTimerManager().SetTimer(
        BalanceTimer,
        this,
        &AScalesOfMaAt::OnBalanceComplete,
        BalanceDuration,
        false
    );

    return true;
}

void AScalesOfMaAt::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the balance radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= BalanceRadius)
        {
            ApplyBalanceEffects(Character);
        }
    }
}

void AScalesOfMaAt::CreateBalanceEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the scales
    // This could include particle effects, post-processing effects, etc.
}

void AScalesOfMaAt::ApplyBalanceEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateBalanceIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EBalanceType::Justice:
            // Apply justice effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.5f);
            break;

        case EBalanceType::Truth:
            // Apply truth effects
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.6f);
            break;

        case EBalanceType::Order:
            // Apply order effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, -Intensity * 0.4f);
            break;

        case EBalanceType::Harmony:
            // Apply harmony effects
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.5f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.7f);
            break;

        case EBalanceType::Maat:
            // Apply perfect cosmic order
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.9f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.8f);
            break;
    }
}

float AScalesOfMaAt::CalculateBalanceIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / BalanceRadius), 0.0f, 1.0f);
    
    return BalanceIntensity * DistanceFactor;
}

void AScalesOfMaAt::OnBalanceComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 