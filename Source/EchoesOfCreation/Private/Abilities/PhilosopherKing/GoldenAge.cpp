#include "Abilities/PhilosopherKing/GoldenAge.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "ResonanceManager.h"
#include "EchoManager.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DecalComponent.h"

AGoldenAge::AGoldenAge()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;

    // Create and setup the enlightenment sphere
    EnlightenmentSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EnlightenmentSphere"));
    RootComponent = EnlightenmentSphere;
    EnlightenmentSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    EnlightenmentSphere->SetGenerateOverlapEvents(true);

    // Default values
    EnlightenmentRange = 1200.0f;
    WisdomPower = 1.0f;
    HarmonyFactor = 1.0f;
}

void AGoldenAge::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.5f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 3.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 3.0f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.5f);

    EnlightenmentSphere->SetSphereRadius(EnlightenmentRange);

    // Load visual effect assets
    if (!AuraEffect)
    {
        AuraEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/GoldenAge/NS_GoldenAgeAura"));
    }
    if (!SymbolsEffect)
    {
        SymbolsEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/GoldenAge/NS_GoldenAgeSymbols"));
    }
    if (!GroundEffect)
    {
        GroundEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/GoldenAge/NS_GoldenAgeGround"));
    }
    if (!AffectedCharacterEffect)
    {
        AffectedCharacterEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/GoldenAge/NS_GoldenAgeAffected"));
    }
}

void AGoldenAge::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateGoldenAgeState(DeltaTime);
        ApplyEnlightenmentEffects(DeltaTime);
    }
}

void AGoldenAge::InitializeGoldenAge(EGoldenAgeType AgeType, float BasePower, float Duration)
{
    CurrentAgeType = AgeType;
    WisdomPower = BasePower;
    AgeDuration = Duration;

    // Adjust properties based on age type
    switch (AgeType)
    {
        case EGoldenAgeType::Enlightenment:
            ResonanceModifiers[EResonanceType::Faith] = 3.5f;
            EchoInteractions[EEchoType::Divine] = 3.8f;
            HarmonyFactor = 1.2f;
            break;

        case EGoldenAgeType::Harmony:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.2f;
            EchoInteractions[EEchoType::Warped] = 3.5f;
            HarmonyFactor = 1.5f;
            break;

        case EGoldenAgeType::Prosperity:
            ResonanceModifiers[EResonanceType::Faith] = 3.0f;
            EchoInteractions[EEchoType::Divine] = 3.2f;
            HarmonyFactor = 1.3f;
            break;

        case EGoldenAgeType::Transcendence:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.8f;
            EchoInteractions[EEchoType::Divine] = 4.0f;
            HarmonyFactor = 1.8f;
            break;
    }
}

void AGoldenAge::ActivateGoldenAge()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnGoldenAgeActivated();

        // Initial activation effects
        if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
        {
            // Store original values
            OriginalTimeScale = OwnerCharacter->CustomTimeDilation;
            
            // Apply initial enlightenment effects
            OwnerCharacter->CustomTimeDilation *= (1.0f + HarmonyFactor * 0.2f);
        }

        // Create initial manifestation effects
        SpawnManifestationEffects();
    }
}

void AGoldenAge::DeactivateGoldenAge()
{
    if (bIsActive)
    {
        bIsActive = false;

        // Restore original values
        if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
        {
            OwnerCharacter->CustomTimeDilation = OriginalTimeScale;
        }

        // Cleanup effects
        CleanupManifestationEffects();
        OnGoldenAgeDeactivated();
    }
}

void AGoldenAge::UpdateGoldenAgeState(float DeltaTime)
{
    // Update duration
    AgeDuration -= DeltaTime;
    if (AgeDuration <= 0.0f)
    {
        DeactivateGoldenAge();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Update manifestation effects
    UpdateManifestationEffects(DeltaTime);
}

void AGoldenAge::ApplyEnlightenmentEffects(float DeltaTime)
{
    if (!GetOwner()) return;

    // Find all actors in enlightenment range
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), EnlightenmentRange, ActorsInRange);

    for (AActor* Actor : ActorsInRange)
    {
        if (!Actor || Actor == GetOwner()) continue;

        ACharacter* Character = Cast<ACharacter>(Actor);
        if (!Character) continue;

        float EffectStrength = CalculateEnlightenmentStrength(Actor);
        ApplyAgeEffects(Character, EffectStrength, DeltaTime);
    }

    // Draw debug sphere if in editor
    #if WITH_EDITOR
    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        EnlightenmentRange,
        32,
        FColor::Yellow,
        false,
        DeltaTime,
        0,
        2.0f
    );
    #endif
}

void AGoldenAge::ApplyAgeEffects(ACharacter* Target, float Strength, float DeltaTime)
{
    if (!Target) return;

    float EffectAmount = Strength * DeltaTime;

    switch (CurrentAgeType)
    {
        case EGoldenAgeType::Enlightenment:
            // Enhanced wisdom and perception
            if (Target->GetCharacterMovement())
            {
                // Increase movement speed and jump height
                Target->GetCharacterMovement()->MaxWalkSpeed *= (1.0f + EffectAmount * 0.3f);
                Target->GetCharacterMovement()->JumpZVelocity *= (1.0f + EffectAmount * 0.2f);
            }
            // Heal gradually
            Target->ModifyHealth(EffectAmount * WisdomPower * 10.0f);
            break;

        case EGoldenAgeType::Harmony:
            // Perfect balance and coordination
            if (Target->GetCharacterMovement())
            {
                // Improve movement control
                Target->GetCharacterMovement()->BrakingDecelerationWalking *= (1.0f + EffectAmount * 0.4f);
                Target->GetCharacterMovement()->GroundFriction *= (1.0f + EffectAmount * 0.3f);
            }
            // Minor healing
            Target->ModifyHealth(EffectAmount * WisdomPower * 5.0f);
            break;

        case EGoldenAgeType::Prosperity:
            // Resource generation and vitality
            if (Target->GetCharacterMovement())
            {
                // Enhanced stamina
                Target->GetCharacterMovement()->MaxAcceleration *= (1.0f + EffectAmount * 0.3f);
            }
            // Significant healing
            Target->ModifyHealth(EffectAmount * WisdomPower * 15.0f);
            break;

        case EGoldenAgeType::Transcendence:
            // Temporary ascension
            if (Target->GetCharacterMovement())
            {
                // Ethereal movement
                Target->GetCharacterMovement()->MaxWalkSpeed *= (1.0f + EffectAmount * 0.5f);
                Target->GetCharacterMovement()->GravityScale *= (1.0f - EffectAmount * 0.3f);
                Target->GetCharacterMovement()->AirControl *= (1.0f + EffectAmount * 0.4f);
            }
            // Continuous healing
            Target->ModifyHealth(EffectAmount * WisdomPower * 20.0f);
            break;
    }

    // Apply common enlightenment effects
    Target->CustomTimeDilation = FMath::Lerp(Target->CustomTimeDilation, 1.0f + (HarmonyFactor * 0.1f), EffectAmount);
}

float AGoldenAge::CalculateEnlightenmentStrength(AActor* Target)
{
    if (!Target || !GetOwner()) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / EnlightenmentRange), 0.0f, 1.0f);

    // Apply age type modifier
    float TypeModifier = 1.0f;
    switch (CurrentAgeType)
    {
        case EGoldenAgeType::Enlightenment:
            TypeModifier = 1.2f;
            break;
        case EGoldenAgeType::Harmony:
            TypeModifier = 1.3f;
            break;
        case EGoldenAgeType::Prosperity:
            TypeModifier = 1.1f;
            break;
        case EGoldenAgeType::Transcendence:
            TypeModifier = 1.5f;
            break;
    }

    return WisdomPower * DistanceFactor * TypeModifier * HarmonyFactor;
}

void AGoldenAge::SpawnManifestationEffects()
{
    if (!GetOwner()) return;

    FVector Location = GetActorLocation();
    FRotator Rotation = GetActorRotation();

    // Spawn aura effect
    if (AuraEffect)
    {
        AuraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            AuraEffect,
            EnlightenmentSphere,
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );

        // Set aura parameters based on age type
        if (AuraComponent)
        {
            FLinearColor AuraColor;
            switch (CurrentAgeType)
            {
                case EGoldenAgeType::Enlightenment:
                    AuraColor = FLinearColor(1.0f, 0.9f, 0.4f, 1.0f); // Golden yellow
                    break;
                case EGoldenAgeType::Harmony:
                    AuraColor = FLinearColor(0.4f, 1.0f, 0.8f, 1.0f); // Cyan
                    break;
                case EGoldenAgeType::Prosperity:
                    AuraColor = FLinearColor(0.8f, 0.4f, 1.0f, 1.0f); // Purple
                    break;
                case EGoldenAgeType::Transcendence:
                    AuraColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // White
                    break;
            }
            AuraComponent->SetVariableLinearColor(FString("AuraColor"), AuraColor);
            AuraComponent->SetVariableFloat(FString("AuraIntensity"), WisdomPower * HarmonyFactor);
        }
    }

    // Spawn floating symbols
    if (SymbolsEffect)
    {
        SymbolsComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            SymbolsEffect,
            Location,
            Rotation
        );

        if (SymbolsComponent)
        {
            // Set symbol parameters based on age type
            FString SymbolSet;
            switch (CurrentAgeType)
            {
                case EGoldenAgeType::Enlightenment:
                    SymbolSet = "Enlightenment";
                    break;
                case EGoldenAgeType::Harmony:
                    SymbolSet = "Harmony";
                    break;
                case EGoldenAgeType::Prosperity:
                    SymbolSet = "Prosperity";
                    break;
                case EGoldenAgeType::Transcendence:
                    SymbolSet = "Transcendence";
                    break;
            }
            SymbolsComponent->SetVariableString(FString("SymbolSet"), SymbolSet);
            SymbolsComponent->SetVariableFloat(FString("SymbolScale"), WisdomPower);
        }
    }

    // Spawn ground effect
    if (GroundEffect)
    {
        GroundComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            GroundEffect,
            Location,
            FRotator(-90.0f, 0.0f, 0.0f)
        );

        if (GroundComponent)
        {
            GroundComponent->SetVariableFloat(FString("Radius"), EnlightenmentRange);
            GroundComponent->SetVariableFloat(FString("Intensity"), HarmonyFactor);
        }
    }

    // Create dynamic material for post-process effects
    if (PostProcessMaterial)
    {
        PostProcessInstance = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
        if (PostProcessInstance)
        {
            // Apply post-process effect to the area
            PostProcessComponent = NewObject<UPostProcessComponent>(this);
            PostProcessComponent->RegisterComponent();
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(
                FWeightedBlendable(1.0f, PostProcessInstance)
            );
            
            // Set material parameters based on age type
            switch (CurrentAgeType)
            {
                case EGoldenAgeType::Enlightenment:
                    PostProcessInstance->SetScalarParameterValue("DistortionIntensity", 0.2f);
                    PostProcessInstance->SetScalarParameterValue("ChromaticAberration", 0.1f);
                    break;
                case EGoldenAgeType::Harmony:
                    PostProcessInstance->SetScalarParameterValue("DistortionIntensity", 0.1f);
                    PostProcessInstance->SetScalarParameterValue("ChromaticAberration", 0.05f);
                    break;
                case EGoldenAgeType::Prosperity:
                    PostProcessInstance->SetScalarParameterValue("DistortionIntensity", 0.15f);
                    PostProcessInstance->SetScalarParameterValue("ChromaticAberration", 0.15f);
                    break;
                case EGoldenAgeType::Transcendence:
                    PostProcessInstance->SetScalarParameterValue("DistortionIntensity", 0.3f);
                    PostProcessInstance->SetScalarParameterValue("ChromaticAberration", 0.2f);
                    break;
            }
        }
    }
}

void AGoldenAge::UpdateManifestationEffects(float DeltaTime)
{
    if (!GetOwner()) return;

    // Update aura effect
    if (AuraComponent)
    {
        float PulseIntensity = (1.0f + FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f)) * 0.5f;
        AuraComponent->SetVariableFloat(FString("PulseIntensity"), PulseIntensity);
        AuraComponent->SetVariableFloat(FString("AuraSize"), EnlightenmentRange * (1.0f + PulseIntensity * 0.1f));
    }

    // Update symbols effect
    if (SymbolsComponent)
    {
        float RotationSpeed = WisdomPower * 60.0f;
        SymbolsComponent->SetVariableFloat(FString("RotationSpeed"), RotationSpeed);
        
        // Update symbol opacity based on power
        float Opacity = FMath::Lerp(0.5f, 1.0f, WisdomPower);
        SymbolsComponent->SetVariableFloat(FString("SymbolOpacity"), Opacity);
    }

    // Update ground effect
    if (GroundComponent)
    {
        float WaveIntensity = HarmonyFactor * (1.0f + FMath::Sin(GetWorld()->GetTimeSeconds() * 1.5f)) * 0.5f;
        GroundComponent->SetVariableFloat(FString("WaveIntensity"), WaveIntensity);
    }

    // Update post-process effect
    if (PostProcessInstance)
    {
        float TimeScale = GetWorld()->GetTimeSeconds();
        float DistortionPulse = FMath::Sin(TimeScale * 1.5f) * 0.1f;
        PostProcessInstance->SetScalarParameterValue("DistortionPulse", DistortionPulse);
    }

    // Update affected character effects
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, GetActorLocation(), EnlightenmentRange, ActorsInRange);

    for (AActor* Actor : ActorsInRange)
    {
        if (ACharacter* Character = Cast<ACharacter>(Actor))
        {
            // Spawn or update character effect if not already present
            UNiagaraComponent* CharacterEffect = nullptr;
            if (!AffectedCharacterEffects.Contains(Character))
            {
                if (AffectedCharacterEffect)
                {
                    CharacterEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
                        AffectedCharacterEffect,
                        Character->GetRootComponent(),
                        NAME_None,
                        FVector::ZeroVector,
                        FRotator::ZeroRotator,
                        EAttachLocation::SnapToTarget,
                        true
                    );
                    AffectedCharacterEffects.Add(Character, CharacterEffect);
                }
            }
            else
            {
                CharacterEffect = AffectedCharacterEffects[Character];
            }

            if (CharacterEffect)
            {
                float EffectStrength = CalculateEnlightenmentStrength(Character);
                CharacterEffect->SetVariableFloat(FString("EffectStrength"), EffectStrength);
            }
        }
    }

    // Remove effects for characters that are no longer in range
    for (auto It = AffectedCharacterEffects.CreateIterator(); It; ++It)
    {
        if (ACharacter* Character = It->Key)
        {
            if (!ActorsInRange.Contains(Character))
            {
                if (UNiagaraComponent* Effect = It->Value)
                {
                    Effect->DestroyComponent();
                }
                It.RemoveCurrent();
            }
        }
    }
}

void AGoldenAge::CleanupManifestationEffects()
{
    // Cleanup aura effect
    if (AuraComponent)
    {
        AuraComponent->DeactivateImmediate();
        AuraComponent = nullptr;
    }

    // Cleanup symbols effect
    if (SymbolsComponent)
    {
        SymbolsComponent->DeactivateImmediate();
        SymbolsComponent = nullptr;
    }

    // Cleanup ground effect
    if (GroundComponent)
    {
        GroundComponent->DeactivateImmediate();
        GroundComponent = nullptr;
    }

    // Cleanup post-process effect
    if (PostProcessComponent)
    {
        PostProcessComponent->UnregisterComponent();
        PostProcessComponent = nullptr;
    }

    // Cleanup character effects
    for (auto& Pair : AffectedCharacterEffects)
    {
        if (UNiagaraComponent* Effect = Pair.Value)
        {
            Effect->DeactivateImmediate();
        }
    }
    AffectedCharacterEffects.Empty();
}

void AGoldenAge::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = WisdomPower * Modifier.Value * ResonanceGenerationRate * HarmonyFactor;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AGoldenAge::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * WisdomPower * HarmonyFactor);
        }
    }
} 