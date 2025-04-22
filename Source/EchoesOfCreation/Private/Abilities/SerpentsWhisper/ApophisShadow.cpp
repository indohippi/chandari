#include "Abilities/SerpentsWhisper/ApophisShadow.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ResonanceManager.h"
#include "EchoManager.h"

AApophisShadow::AApophisShadow()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    ShadowRange = 800.0f;
    StealthFactor = 1.0f;
}

void AApophisShadow::BeginPlay()
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

void AApophisShadow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateShadowState(DeltaTime);
        ApplyShadowEffects();
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
        case EShadowType::Deception:
            StealthFactor = 1.2f;
            ResonanceModifiers[EResonanceType::Doubt] = 3.0f;
            EchoInteractions[EEchoType::Corrupted] = 2.8f;
            break;
        case EShadowType::Concealment:
            StealthFactor = 1.5f;
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.5f;
            break;
        case EShadowType::Chaos:
            StealthFactor = 1.3f;
            ResonanceModifiers[EResonanceType::Curiosity] = 2.8f;
            EchoInteractions[EEchoType::Warped] = 2.8f;
            break;
    }
}

void AApophisShadow::ActivateShadow()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnShadowActivated();
        
        // Apply initial stealth effects
        if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
        {
            // Modify collision and visibility
            OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
            OwnerCharacter->SetActorHiddenInGame(true);
            
            // Store original values for restoration
            OriginalWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
            OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= (1.0f + StealthFactor * 0.2f);
        }
    }
}

void AApophisShadow::DeactivateShadow()
{
    if (bIsActive)
    {
        bIsActive = false;
        
        // Restore original state
        if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
        {
            OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
            OwnerCharacter->SetActorHiddenInGame(false);
            OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
        }
        
        OnShadowDeactivated();
    }
}

void AApophisShadow::ApplyShadowEffects()
{
    if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
    {
        switch (CurrentShadowType)
        {
            case EShadowType::Deception:
                // Create illusory duplicates or misleading effects
                CreateDeceptionEffects();
                break;
                
            case EShadowType::Concealment:
                // Enhanced stealth and noise reduction
                ApplyConcealmentEffects(OwnerCharacter);
                break;
                
            case EShadowType::Chaos:
                // Disruptive effects on nearby enemies
                ApplyChaosEffects();
                break;
        }
    }
}

void AApophisShadow::CreateDeceptionEffects()
{
    // Spawn illusory duplicates at random positions around the player
    if (FMath::Rand() % 100 < 10) // 10% chance per tick
    {
        FVector RandomOffset = FVector(
            FMath::RandRange(-200.0f, 200.0f),
            FMath::RandRange(-200.0f, 200.0f),
            0.0f
        );
        
        FVector SpawnLocation = GetOwner()->GetActorLocation() + RandomOffset;
        // TODO: Spawn actual illusion actor
        OnIllusionCreated(SpawnLocation);
    }
}

void AApophisShadow::ApplyConcealmentEffects(ACharacter* OwnerCharacter)
{
    // Reduce visibility based on movement
    float MovementFactor = OwnerCharacter->GetVelocity().Size() / OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;
    float VisibilityFactor = FMath::Lerp(0.0f, 1.0f, MovementFactor);
    
    // Apply visibility changes
    OwnerCharacter->CustomTimeDilation = FMath::Lerp(0.8f, 1.0f, VisibilityFactor);
}

void AApophisShadow::ApplyChaosEffects()
{
    // Find nearby enemies and apply disruptive effects
    TArray<AActor*> NearbyActors;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), ShadowRange, NearbyActors);
    
    for (AActor* Actor : NearbyActors)
    {
        if (Actor != GetOwner() && Cast<ACharacter>(Actor))
        {
            // Apply confusion or disorientation effect
            if (ACharacter* EnemyCharacter = Cast<ACharacter>(Actor))
            {
                // Randomly modify their movement
                if (FMath::Rand() % 100 < 20) // 20% chance per tick
                {
                    FRotator NewRotation = EnemyCharacter->GetActorRotation();
                    NewRotation.Yaw += FMath::RandRange(-30.0f, 30.0f);
                    EnemyCharacter->SetActorRotation(NewRotation);
                }
            }
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