#include "Abilities/YinYangShift.h"
#include "Characters/EchoesCharacter.h"
#include "Components/ResonanceComponent.h"
#include "Components/EchoComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AYinYangShift::AYinYangShift()
{
    // Set default values
    AbilityName = TEXT("Yin & Yang Shift");
    ResonanceCost = 0.35f;
    Cooldown = 20.0f;
    Range = 800.0f;
    SelectedType = EShiftType::Balance;
}

void AYinYangShift::SelectShiftType(EShiftType Type)
{
    SelectedType = Type;
}

bool AYinYangShift::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create shift effect
    CreateShiftEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Set timer for shift duration
    GetWorld()->GetTimerManager().SetTimer(
        ShiftTimer,
        this,
        &AYinYangShift::OnShiftComplete,
        ShiftDuration,
        false
    );

    return true;
}

void AYinYangShift::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters within the shift radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character && FVector::Dist(Character->GetActorLocation(), TargetLocation) <= ShiftRadius)
        {
            ApplyShiftEffects(Character);
        }
    }
}

void AYinYangShift::CreateShiftEffect(const FVector& Center)
{
    // TODO: Implement visual effects for the Yin & Yang shift
    // This could include particle effects, post-processing effects, etc.
}

void AYinYangShift::ApplyShiftEffects(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return;
    }

    float Intensity = CalculateShiftIntensity(Target);
    UResonanceComponent* ResonanceComp = Target->GetResonanceComponent();
    UEchoComponent* EchoComp = Target->GetEchoComponent();

    if (!ResonanceComp || !EchoComp)
    {
        return;
    }

    switch (SelectedType)
    {
        case EShiftType::YinToYang:
            // Transform dark to light
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, -Intensity * 0.7f);
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.6f);
            break;

        case EShiftType::YangToYin:
            // Transform light to dark
            ResonanceComp->GenerateResonance(EResonanceType::Faith, -Intensity * 0.7f);
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::RealityWarping, Intensity * 0.6f);
            break;

        case EShiftType::Balance:
            // Maintain equilibrium
            ResonanceComp->GenerateResonance(EResonanceType::Curiosity, Intensity * 0.6f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.5f);
            break;

        case EShiftType::Duality:
            // Channel both energies
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.4f);
            ResonanceComp->GenerateResonance(EResonanceType::Doubt, Intensity * 0.4f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.5f);
            break;

        case EShiftType::Dragon:
            // Channel through draconic power
            ResonanceComp->GenerateResonance(EResonanceType::Faith, Intensity * 0.8f);
            EchoComp->ApplyEchoEffect(EEchoType::KnowledgeTransfer, Intensity * 0.7f);
            EchoComp->ApplyEchoEffect(EEchoType::HarmonyRestoration, Intensity * 0.6f);
            break;
    }
}

float AYinYangShift::CalculateShiftIntensity(AEchoesCharacter* Target)
{
    if (!Target)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / ShiftRadius), 0.0f, 1.0f);
    
    return ShiftIntensity * DistanceFactor;
}

void AYinYangShift::OnShiftComplete()
{
    // Clean up any ongoing effects
    // TODO: Implement cleanup logic for visual effects and ongoing effects
} 