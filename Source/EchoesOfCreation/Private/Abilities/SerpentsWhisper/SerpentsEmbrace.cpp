#include "Abilities/SerpentsWhisper/SerpentsEmbrace.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

ASerpentsEmbrace::ASerpentsEmbrace()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    EmbraceRange = 600.0f;
    ConstrictionPower = 1.0f;
    MaxTargets = 3;
}

void ASerpentsEmbrace::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 1.5f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 2.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.0f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 1.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 2.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);
}

void ASerpentsEmbrace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateEmbraceState(DeltaTime);
        ApplyConstrictionEffects(DeltaTime);
    }
}

void ASerpentsEmbrace::InitializeEmbrace(EEmbraceType EmbraceType, float BasePower, float Duration)
{
    CurrentEmbraceType = EmbraceType;
    ConstrictionPower = BasePower;
    EmbraceDuration = Duration;

    // Adjust properties based on embrace type
    switch (EmbraceType)
    {
        case EEmbraceType::Constriction:
            // Maximum control over fewer targets
            MaxTargets = 2;
            ConstrictionPower *= 1.5f;
            ResonanceModifiers[EResonanceType::Doubt] = 3.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.8f;
            break;

        case EEmbraceType::Envelopment:
            // Moderate control over more targets
            MaxTargets = 4;
            ConstrictionPower *= 1.2f;
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.5f;
            break;

        case EEmbraceType::Absorption:
            // Energy drain and healing
            MaxTargets = 3;
            ConstrictionPower *= 1.3f;
            ResonanceModifiers[EResonanceType::Curiosity] = 2.8f;
            EchoInteractions[EEchoType::Warped] = 2.8f;
            break;
    }
}

void ASerpentsEmbrace::ActivateEmbrace()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnEmbraceActivated();
        FindTargetsInRange();
    }
}

void ASerpentsEmbrace::DeactivateEmbrace()
{
    if (bIsActive)
    {
        bIsActive = false;
        
        // Release all constricted targets
        for (auto& Target : ConstrictedTargets)
        {
            ReleaseTarget(Target.Key);
        }
        ConstrictedTargets.Empty();
        
        OnEmbraceDeactivated();
    }
}

void ASerpentsEmbrace::FindTargetsInRange()
{
    TArray<AActor*> PotentialTargets;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), EmbraceRange, PotentialTargets);

    for (AActor* Actor : PotentialTargets)
    {
        if (CanConstrictTarget(Actor) && ConstrictedTargets.Num() < MaxTargets)
        {
            ConstrictTarget(Actor);
        }
    }
}

bool ASerpentsEmbrace::CanConstrictTarget(AActor* Target)
{
    if (!Target || Target == GetOwner() || ConstrictedTargets.Contains(Target)) return false;

    ACharacter* Character = Cast<ACharacter>(Target);
    return Character != nullptr;
}

void ASerpentsEmbrace::ConstrictTarget(AActor* Target)
{
    if (!Target) return;

    ACharacter* Character = Cast<ACharacter>(Target);
    if (!Character) return;

    // Store original movement properties
    FConstrictedTargetData TargetData;
    TargetData.OriginalMaxSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
    TargetData.ConstrictionTime = 0.0f;
    TargetData.ConstrictionStrength = CalculateConstrictionStrength(Target);

    ConstrictedTargets.Add(Target, TargetData);

    // Apply initial constriction effects
    Character->GetCharacterMovement()->MaxWalkSpeed *= (1.0f - TargetData.ConstrictionStrength);
    Character->GetCharacterMovement()->GravityScale *= (1.0f + TargetData.ConstrictionStrength);

    OnTargetConstricted(Target, TargetData.ConstrictionStrength);
}

void ASerpentsEmbrace::ReleaseTarget(AActor* Target)
{
    if (!Target) return;

    ACharacter* Character = Cast<ACharacter>(Target);
    if (!Character) return;

    // Restore original movement properties
    if (FConstrictedTargetData* TargetData = ConstrictedTargets.Find(Target))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed = TargetData->OriginalMaxSpeed;
        Character->GetCharacterMovement()->GravityScale = 1.0f;
    }

    OnTargetReleased(Target);
}

void ASerpentsEmbrace::ApplyConstrictionEffects(float DeltaTime)
{
    TArray<AActor*> TargetsToRelease;

    for (auto& TargetPair : ConstrictedTargets)
    {
        AActor* Target = TargetPair.Key;
        FConstrictedTargetData& TargetData = TargetPair.Value;

        if (!Target || !IsValid(Target))
        {
            TargetsToRelease.Add(Target);
            continue;
        }

        TargetData.ConstrictionTime += DeltaTime;
        float CurrentStrength = TargetData.ConstrictionStrength;

        ACharacter* Character = Cast<ACharacter>(Target);
        if (!Character) continue;

        switch (CurrentEmbraceType)
        {
            case EEmbraceType::Constriction:
                // Apply increasing constriction damage
                ApplyConstrictionDamage(Character, CurrentStrength, DeltaTime);
                // Gradually reduce movement speed
                Character->GetCharacterMovement()->MaxWalkSpeed *= (1.0f - CurrentStrength * 0.1f);
                break;

            case EEmbraceType::Envelopment:
                // Apply moderate damage and control effects
                ApplyEnvelopmentEffects(Character, CurrentStrength, DeltaTime);
                break;

            case EEmbraceType::Absorption:
                // Drain health and resources
                ApplyAbsorptionEffects(Character, CurrentStrength, DeltaTime);
                break;
        }

        // Check if target has moved out of range
        float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
        if (Distance > EmbraceRange)
        {
            TargetsToRelease.Add(Target);
        }
    }

    // Release targets that are no longer valid or in range
    for (AActor* Target : TargetsToRelease)
    {
        ReleaseTarget(Target);
        ConstrictedTargets.Remove(Target);
    }
}

void ASerpentsEmbrace::ApplyConstrictionDamage(ACharacter* Target, float Strength, float DeltaTime)
{
    float DamageAmount = ConstrictionPower * Strength * DeltaTime;
    UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, this, nullptr);

    // Apply additional movement restrictions
    if (Target->GetCharacterMovement())
    {
        Target->GetCharacterMovement()->GravityScale = FMath::Min(
            Target->GetCharacterMovement()->GravityScale + (Strength * DeltaTime),
            2.0f
        );
    }
}

void ASerpentsEmbrace::ApplyEnvelopmentEffects(ACharacter* Target, float Strength, float DeltaTime)
{
    // Apply moderate damage
    float DamageAmount = ConstrictionPower * Strength * DeltaTime * 0.5f;
    UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, this, nullptr);

    // Apply control effects
    if (Target->GetCharacterMovement())
    {
        // Gradually pull target towards the caster
        FVector DirectionToCaster = (GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
        Target->GetCharacterMovement()->AddForce(DirectionToCaster * Strength * 1000.0f);
    }
}

void ASerpentsEmbrace::ApplyAbsorptionEffects(ACharacter* Target, float Strength, float DeltaTime)
{
    // Apply health drain
    float DrainAmount = ConstrictionPower * Strength * DeltaTime * 0.7f;
    UGameplayStatics::ApplyDamage(Target, DrainAmount, nullptr, this, nullptr);

    // Heal the caster
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        if (OwnerCharacter->GetHealthComponent())
        {
            OwnerCharacter->GetHealthComponent()->Heal(DrainAmount * 0.5f);
        }
    }

    // Apply energy drain effects
    if (Target->GetCharacterMovement())
    {
        Target->GetCharacterMovement()->MaxWalkSpeed *= (1.0f - Strength * 0.05f);
    }
}

float ASerpentsEmbrace::CalculateConstrictionStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / EmbraceRange), 0.0f, 1.0f);

    // Apply embrace type modifier
    float TypeModifier = 1.0f;
    switch (CurrentEmbraceType)
    {
        case EEmbraceType::Constriction:
            TypeModifier = 1.5f;
            break;
        case EEmbraceType::Envelopment:
            TypeModifier = 1.2f;
            break;
        case EEmbraceType::Absorption:
            TypeModifier = 1.3f;
            break;
    }

    return ConstrictionPower * DistanceFactor * TypeModifier;
}

void ASerpentsEmbrace::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ConstrictionPower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void ASerpentsEmbrace::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ConstrictionPower);
        }
    }
}

void ASerpentsEmbrace::UpdateEmbraceState(float DeltaTime)
{
    // Update embrace duration
    EmbraceDuration -= DeltaTime;
    if (EmbraceDuration <= 0.0f)
    {
        DeactivateEmbrace();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();
} 