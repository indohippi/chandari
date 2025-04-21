#include "Abilities/SerpentsWhisper/ApophisShadow.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AApophisShadow::AApophisShadow()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
}

void AApophisShadow::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ShadowRadius = 500.0f;
    ResonanceGenerationRate = 1.2f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 0.3f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 0.8f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 0.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 2.0f);
    EchoInteractions.Add(EEchoType::Warped, 1.5f);
}

void AApophisShadow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateShadowState(DeltaTime);
        CheckAffectedActors();
    }
}

void AApophisShadow::InitializeShadow(EShadowType ShadowType, float BasePower, float Duration)
{
    CurrentShadowType = ShadowType;
    ShadowPower = BasePower;
    ShadowDuration = Duration;

    // Adjust properties based on shadow type
    switch (ShadowType)
    {
        case EShadowType::CorruptingDarkness:
            ResonanceModifiers[EResonanceType::Doubt] = 2.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.5f;
            break;
        case EShadowType::ConsumingVoid:
            ResonanceModifiers[EResonanceType::Curiosity] = 1.5f;
            EchoInteractions[EEchoType::Warped] = 2.0f;
            break;
        case EShadowType::TwistingShadows:
            ResonanceModifiers[EResonanceType::Faith] = 0.1f;
            EchoInteractions[EEchoType::Divine] = 0.2f;
            break;
    }
}

void AApophisShadow::ActivateShadow()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnShadowActivated();
        CheckAffectedActors();
    }
}

void AApophisShadow::DeactivateShadow()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnShadowDeactivated();
    }
}

void AApophisShadow::ExpandShadow(float ExpansionRate)
{
    ShadowRadius += ExpansionRate;
    OnShadowExpanded(ShadowRadius);
}

void AApophisShadow::ApplyShadowEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        switch (CurrentShadowType)
        {
            case EShadowType::CorruptingDarkness:
                // Apply damage over time and corruption
                UGameplayStatics::ApplyDamage(Target, ShadowPower * 0.1f, nullptr, this, nullptr);
                // Apply corruption effect (to be implemented in character class)
                break;

            case EShadowType::ConsumingVoid:
                // Apply void damage and movement speed reduction
                UGameplayStatics::ApplyDamage(Target, ShadowPower * 0.15f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.7f;
                }
                break;

            case EShadowType::TwistingShadows:
                // Apply confusion and minor damage
                UGameplayStatics::ApplyDamage(Target, ShadowPower * 0.05f, nullptr, this, nullptr);
                // Apply confusion effect (to be implemented in character class)
                break;
        }
    }
}

void AApophisShadow::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ShadowPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AApophisShadow::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ShadowPower);
        }
    }
}

void AApophisShadow::UpdateShadowState(float DeltaTime)
{
    // Update shadow duration
    ShadowDuration -= DeltaTime;
    if (ShadowDuration <= 0.0f)
    {
        DeactivateShadow();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();
}

void AApophisShadow::CheckAffectedActors()
{
    // Get all actors in the shadow radius
    TArray<AActor*> OverlappingActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());

    // Get all actors in the shadow radius
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        GetActorLocation(),
        ShadowRadius,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappingActors
    );

    // Apply effects to all affected actors
    for (AActor* Actor : OverlappingActors)
    {
        ApplyShadowEffects(Actor);
    }
} 