#include "Abilities/JadeDragon/DragonsBreath.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ADragonsBreath::ADragonsBreath()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
}

void ADragonsBreath::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    BreathRange = 1000.0f;
    BreathWidth = 200.0f;
    ResonanceGenerationRate = 1.2f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 0.8f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 1.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.2f);
    EchoInteractions.Add(EEchoType::Corrupted, 0.7f);
    EchoInteractions.Add(EEchoType::Warped, 1.8f);
}

void ADragonsBreath::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateBreathState(DeltaTime);
        CheckAffectedActors();
    }
}

void ADragonsBreath::InitializeBreath(EBreathType BreathType, float BasePower, float Duration)
{
    CurrentBreathType = BreathType;
    BreathPower = BasePower;
    BreathDuration = Duration;

    // Adjust properties based on breath type
    switch (BreathType)
    {
        case EBreathType::CelestialFire:
            ResonanceModifiers[EResonanceType::Faith] = 1.8f;
            EchoInteractions[EEchoType::Divine] = 2.0f;
            break;
        case EBreathType::AbyssalFrost:
            ResonanceModifiers[EResonanceType::Doubt] = 1.5f;
            EchoInteractions[EEchoType::Corrupted] = 1.8f;
            break;
        case EBreathType::EternalStorm:
            ResonanceModifiers[EResonanceType::Curiosity] = 2.0f;
            EchoInteractions[EEchoType::Warped] = 2.2f;
            break;
    }
}

void ADragonsBreath::ActivateBreath()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnBreathActivated();
    }
}

void ADragonsBreath::DeactivateBreath()
{
    if (bIsActive)
    {
        bIsActive = false;
        OnBreathDeactivated();
    }
}

void ADragonsBreath::TargetBreath(FVector TargetLocation)
{
    if (!bIsActive) return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), TargetLocation);
        if (Distance <= BreathRange)
        {
            CurrentTargetLocation = TargetLocation;
            OnBreathTargeted(TargetLocation);
            CheckAffectedActors();
        }
    }
}

void ADragonsBreath::ApplyBreathEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        switch (CurrentBreathType)
        {
            case EBreathType::CelestialFire:
                // Apply fire damage and burning effect
                UGameplayStatics::ApplyDamage(Target, BreathPower * 0.15f, nullptr, this, nullptr);
                // Apply burning effect (to be implemented in character class)
                break;

            case EBreathType::AbyssalFrost:
                // Apply frost damage and slow effect
                UGameplayStatics::ApplyDamage(Target, BreathPower * 0.12f, nullptr, this, nullptr);
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.6f;
                }
                break;

            case EBreathType::EternalStorm:
                // Apply storm damage and stun effect
                UGameplayStatics::ApplyDamage(Target, BreathPower * 0.1f, nullptr, this, nullptr);
                // Apply stun effect (to be implemented in character class)
                break;
        }
    }
}

void ADragonsBreath::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = BreathPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void ADragonsBreath::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * BreathPower);
        }
    }
}

void ADragonsBreath::UpdateBreathState(float DeltaTime)
{
    // Update breath duration
    BreathDuration -= DeltaTime;
    if (BreathDuration <= 0.0f)
    {
        DeactivateBreath();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();
}

void ADragonsBreath::CheckAffectedActors()
{
    if (!bIsActive) return;

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    // Get all actors in the breath cone
    TArray<AActor*> OverlappingActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerCharacter);

    // Get all actors in the breath cone
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        CurrentTargetLocation,
        BreathWidth,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappingActors
    );

    // Apply effects to all affected actors
    for (AActor* Actor : OverlappingActors)
    {
        ApplyBreathEffects(Actor);
    }
} 