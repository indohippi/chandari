#include "Abilities/JadeDragon/DragonsBreath.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ResonanceManager.h"
#include "EchoManager.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"

ADragonsBreath::ADragonsBreath()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    BreathRange = 1000.0f;
    BreathAngle = 60.0f;
    ElementalPower = 1.0f;

    // Create root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Create particle system component for the breath effect
    BreathParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BreathParticleSystem"));
    BreathParticleSystem->SetupAttachment(RootComponent);
    BreathParticleSystem->bAutoActivate = false;

    // Create Niagara component for advanced effects
    BreathNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BreathNiagaraSystem"));
    BreathNiagaraSystem->SetupAttachment(RootComponent);
    BreathNiagaraSystem->bAutoActivate = false;
}

void ADragonsBreath::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.0f);
    ResonanceModifiers.Add(EResonanceType::Doubt, 1.5f);
    ResonanceModifiers.Add(EResonanceType::Curiosity, 2.5f);

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.5f);
    EchoInteractions.Add(EEchoType::Warped, 2.0f);

    // Setup combo transitions
    ComboTransitions.Add(EBreathType::CelestialFlame, EBreathType::StormWind);
    ComboTransitions.Add(EBreathType::StormWind, EBreathType::VoidFrost);
    ComboTransitions.Add(EBreathType::VoidFrost, EBreathType::EarthForce);
    ComboTransitions.Add(EBreathType::EarthForce, EBreathType::CelestialFlame);

    // Load visual effect assets
    LoadVisualEffectAssets();
}

void ADragonsBreath::LoadVisualEffectAssets()
{
    // Load particle systems for each breath type
    static ConstructorHelpers::FObjectFinder<UParticleSystem> CelestialFlameFX(TEXT("/Game/Effects/Dragons/P_CelestialFlame"));
    CelestialFlameEffect = CelestialFlameFX.Object;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> VoidFrostFX(TEXT("/Game/Effects/Dragons/P_VoidFrost"));
    VoidFrostEffect = VoidFrostFX.Object;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> StormWindFX(TEXT("/Game/Effects/Dragons/P_StormWind"));
    StormWindEffect = StormWindFX.Object;

    static ConstructorHelpers::FObjectFinder<UParticleSystem> EarthForceFX(TEXT("/Game/Effects/Dragons/P_EarthForce"));
    EarthForceEffect = EarthForceFX.Object;

    // Load Niagara systems
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BreathTrailFX(TEXT("/Game/Effects/Dragons/NS_BreathTrail"));
    BreathTrailEffect = BreathTrailFX.Object;

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BreathImpactFX(TEXT("/Game/Effects/Dragons/NS_BreathImpact"));
    BreathImpactEffect = BreathImpactFX.Object;

    // Load sound effects
    static ConstructorHelpers::FObjectFinder<USoundCue> BreathSoundFX(TEXT("/Game/Sounds/Dragons/SC_DragonBreath"));
    BreathSound = BreathSoundFX.Object;
}

void ADragonsBreath::InitializeBreath(EBreathType BreathType, float BasePower, float Duration)
{
    // Update combo system
    UpdateComboSystem(BreathType);

    CurrentBreathType = BreathType;
    ElementalPower = BasePower * ComboMultiplier;
    BreathDuration = Duration;

    // Set up visual effects based on breath type
    switch (BreathType)
    {
        case EBreathType::CelestialFlame:
            BreathParticleSystem->SetTemplate(CelestialFlameEffect);
            BreathParticleSystem->SetColorParameter(TEXT("BreathColor"), FLinearColor(1.0f, 0.8f, 0.2f, 1.0f));
            break;
        case EBreathType::VoidFrost:
            BreathParticleSystem->SetTemplate(VoidFrostEffect);
            BreathParticleSystem->SetColorParameter(TEXT("BreathColor"), FLinearColor(0.2f, 0.8f, 1.0f, 1.0f));
            break;
        case EBreathType::StormWind:
            BreathParticleSystem->SetTemplate(StormWindEffect);
            BreathParticleSystem->SetColorParameter(TEXT("BreathColor"), FLinearColor(0.7f, 0.7f, 1.0f, 0.8f));
            break;
        case EBreathType::EarthForce:
            BreathParticleSystem->SetTemplate(EarthForceEffect);
            BreathParticleSystem->SetColorParameter(TEXT("BreathColor"), FLinearColor(0.6f, 0.4f, 0.2f, 1.0f));
            break;
    }

    // Set up Niagara trail effect
    if (BreathTrailEffect)
    {
        BreathNiagaraSystem->SetAsset(BreathTrailEffect);
        BreathNiagaraSystem->SetVariableFloat(TEXT("TrailWidth"), BasePower * 50.0f * ComboMultiplier);
        BreathNiagaraSystem->SetVariableFloat(TEXT("TrailLength"), Duration * 100.0f);
    }

    // Adjust properties based on breath type
    switch (BreathType)
    {
        case EBreathType::CelestialFlame:
            ResonanceModifiers[EResonanceType::Faith] = 3.0f;
            EchoInteractions[EEchoType::Divine] = 3.5f;
            break;
        case EBreathType::VoidFrost:
            ResonanceModifiers[EResonanceType::Doubt] = 2.8f;
            EchoInteractions[EEchoType::Corrupted] = 2.5f;
            break;
        case EBreathType::StormWind:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.2f;
            EchoInteractions[EEchoType::Warped] = 3.0f;
            break;
        case EBreathType::EarthForce:
            ResonanceModifiers[EResonanceType::Faith] = 2.5f;
            EchoInteractions[EEchoType::Divine] = 2.8f;
            break;
    }
}

void ADragonsBreath::UpdateComboSystem(EBreathType NewType)
{
    if (CurrentComboCount > 0)
    {
        // Check if this is a valid combo transition
        EBreathType* ExpectedType = ComboTransitions.Find(CurrentBreathType);
        if (ExpectedType && *ExpectedType == NewType)
        {
            // Valid combo transition
            CurrentComboCount++;
            ComboMultiplier = 1.0f + (CurrentComboCount * 0.2f);
            
            // Reset combo timer
            GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
            GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ADragonsBreath::ResetCombo, ComboWindow, false);
            
            OnComboProgress(CurrentComboCount, ComboMultiplier);
        }
        else
        {
            // Invalid transition, reset combo
            ResetCombo();
        }
    }
    else
    {
        // Start new combo
        CurrentComboCount = 1;
        ComboMultiplier = 1.2f;
        GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &ADragonsBreath::ResetCombo, ComboWindow, false);
        OnComboProgress(CurrentComboCount, ComboMultiplier);
    }
}

void ADragonsBreath::ResetCombo()
{
    CurrentComboCount = 0;
    ComboMultiplier = 1.0f;
    OnComboReset();
}

void ADragonsBreath::ActivateBreath()
{
    if (!bIsActive)
    {
        bIsActive = true;

        // Start visual effects
        if (BreathParticleSystem)
        {
            BreathParticleSystem->Activate(true);
        }
        if (BreathNiagaraSystem)
        {
            BreathNiagaraSystem->Activate(true);
        }

        // Play breath sound
        if (BreathSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, BreathSound, GetActorLocation(), GetActorRotation());
        }

        OnBreathActivated();
    }
}

void ADragonsBreath::DeactivateBreath()
{
    if (bIsActive)
    {
        bIsActive = false;

        // Stop visual effects
        if (BreathParticleSystem)
        {
            BreathParticleSystem->Deactivate();
        }
        if (BreathNiagaraSystem)
        {
            BreathNiagaraSystem->Deactivate();
        }

        OnBreathDeactivated();
    }
}

void ADragonsBreath::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateBreathState(DeltaTime);
        ApplyBreathEffects(DeltaTime);
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

void ADragonsBreath::ApplyBreathEffects(float DeltaTime)
{
    if (!GetOwner()) return;

    // Get the forward vector of the breath
    FVector BreathOrigin = GetOwner()->GetActorLocation();
    FVector BreathDirection = GetOwner()->GetActorForwardVector();

    // Find all actors in range
    TArray<AActor*> ActorsInRange;
    UGameplayStatics::GetAllActorsInRadius(this, BreathOrigin, BreathRange, ActorsInRange);

    for (AActor* Actor : ActorsInRange)
    {
        if (!Actor || Actor == GetOwner()) continue;

        // Check if actor is within breath cone
        FVector DirectionToActor = (Actor->GetActorLocation() - BreathOrigin).GetSafeNormal();
        float AngleToActor = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(BreathDirection, DirectionToActor)));

        if (AngleToActor <= BreathAngle * 0.5f)
        {
            ApplyElementalEffects(Actor, DeltaTime);
        }
    }

    // Draw debug cone if in debug mode
    #if WITH_EDITOR
    DrawDebugCone(
        GetWorld(),
        BreathOrigin,
        BreathDirection,
        BreathRange,
        FMath::DegreesToRadians(BreathAngle * 0.5f),
        FMath::DegreesToRadians(BreathAngle * 0.5f),
        12,
        FColor::Red,
        false,
        DeltaTime,
        0,
        2.0f
    );
    #endif
}

void ADragonsBreath::ApplyElementalEffects(AActor* Target, float DeltaTime)
{
    if (!Target) return;

    ACharacter* Character = Cast<ACharacter>(Target);
    if (!Character) return;

    float EffectStrength = CalculateEffectStrength(Target);
    float DamageAmount = EffectStrength * DeltaTime * 20.0f;

    // Apply combo effects
    ApplyComboEffects(Target, DeltaTime);

    // Handle terrain interactions
    HandleTerrainInteractions(Target);

    // Spawn impact effects at target location
    FVector ImpactLocation = Target->GetActorLocation();
    
    // Create environmental effects
    CreateEnvironmentalEffects(ImpactLocation);
    
    // Spawn impact effect
    if (BreathImpactEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            BreathImpactEffect,
            ImpactLocation,
            FRotator::ZeroRotator,
            FVector(EffectStrength),
            true,
            true,
            ENCPoolMethod::AutoRelease,
            true
        );
    }
    
    switch (CurrentBreathType)
    {
        case EBreathType::CelestialFlame:
            // Spawn celestial flame impact effect
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                CelestialFlameEffect,
                ImpactLocation,
                FRotator::ZeroRotator,
                FVector(EffectStrength),
                true,
                EPSCPoolMethod::AutoRelease
            );
            
            // Divine fire damage and purification
            UGameplayStatics::ApplyDamage(Target, DamageAmount, nullptr, this, nullptr);
            if (Character->GetCharacterMovement())
            {
                // Slow effect from divine flames
                Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f;
                
                // Add burning status effect
                if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Character))
                {
                    EchoesChar->AddStatusEffect(EStatusEffectType::Burning, EffectStrength * 0.5f, 3.0f);
                }
            }
            break;

        case EBreathType::VoidFrost:
            // Spawn frost impact effect
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                VoidFrostEffect,
                ImpactLocation,
                FRotator::ZeroRotator,
                FVector(EffectStrength),
                true,
                EPSCPoolMethod::AutoRelease
            );
            
            // Frost damage and movement impairment
            UGameplayStatics::ApplyDamage(Target, DamageAmount * 0.8f, nullptr, this, nullptr);
            if (Character->GetCharacterMovement())
            {
                // Severe movement reduction from frost
                Character->GetCharacterMovement()->MaxWalkSpeed *= 0.6f;
                Character->GetCharacterMovement()->JumpZVelocity *= 0.7f;
                
                // Add freezing status effect
                if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Character))
                {
                    EchoesChar->AddStatusEffect(EStatusEffectType::Freezing, EffectStrength * 0.7f, 2.0f);
                }
            }
            break;

        case EBreathType::StormWind:
            // Spawn wind impact effect
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                StormWindEffect,
                ImpactLocation,
                FRotator::ZeroRotator,
                FVector(EffectStrength),
                true,
                EPSCPoolMethod::AutoRelease
            );
            
            // Wind damage and knockback
            UGameplayStatics::ApplyDamage(Target, DamageAmount * 0.6f, nullptr, this, nullptr);
            if (Character->GetCharacterMovement())
            {
                // Apply force in breath direction
                FVector KnockbackDirection = (Target->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
                Character->GetCharacterMovement()->AddImpulse(KnockbackDirection * EffectStrength * 1000.0f, true);
                
                // Add disorientation status effect
                if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Character))
                {
                    EchoesChar->AddStatusEffect(EStatusEffectType::Disoriented, EffectStrength * 0.4f, 1.5f);
                }
            }
            break;

        case EBreathType::EarthForce:
            // Spawn earth impact effect
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                EarthForceEffect,
                ImpactLocation,
                FRotator::ZeroRotator,
                FVector(EffectStrength),
                true,
                EPSCPoolMethod::AutoRelease
            );
            
            // Earth damage and stagger
            UGameplayStatics::ApplyDamage(Target, DamageAmount * 1.2f, nullptr, this, nullptr);
            
            // Add stagger status effect
            if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Character))
            {
                EchoesChar->AddStatusEffect(EStatusEffectType::Staggered, EffectStrength * 0.6f, 1.0f);
            }
            break;
    }

    // Spawn Niagara trail effect between breath source and target
    if (BreathTrailEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            BreathTrailEffect,
            GetOwner()->GetActorLocation(),
            GetOwner()->GetActorRotation(),
            FVector(EffectStrength),
            true,
            true,
            ENCPoolMethod::AutoRelease,
            true
        );
    }
}

void ADragonsBreath::ApplyComboEffects(AActor* Target, float DeltaTime)
{
    if (CurrentComboCount > 0)
    {
        float ComboEffectStrength = EffectStrength * (ComboMultiplier - 1.0f);
        
        switch (CurrentBreathType)
        {
            case EBreathType::CelestialFlame:
                // Enhanced burning effect
                if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Target))
                {
                    EchoesChar->AddStatusEffect(EStatusEffectType::Burning, ComboEffectStrength * 0.8f, 4.0f);
                }
                break;
                
            case EBreathType::VoidFrost:
                // Enhanced freezing effect
                if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Target))
                {
                    EchoesChar->AddStatusEffect(EStatusEffectType::Freezing, ComboEffectStrength * 1.0f, 3.0f);
                }
                break;
                
            case EBreathType::StormWind:
                // Enhanced knockback
                if (Character->GetCharacterMovement())
                {
                    FVector KnockbackDirection = (Target->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
                    Character->GetCharacterMovement()->AddImpulse(KnockbackDirection * ComboEffectStrength * 1500.0f, true);
                }
                break;
                
            case EBreathType::EarthForce:
                // Enhanced stagger
                if (AEchoesCharacter* EchoesChar = Cast<AEchoesCharacter>(Target))
                {
                    EchoesChar->AddStatusEffect(EStatusEffectType::Staggered, ComboEffectStrength * 0.8f, 2.0f);
                }
                break;
        }
    }
}

void ADragonsBreath::HandleTerrainInteractions(AActor* Target)
{
    if (!Target) return;

    FVector TargetLocation = Target->GetActorLocation();
    
    // Modify terrain around target
    ModifyTerrain(TargetLocation);
    
    // Create environmental effects
    CreateEnvironmentalEffects(TargetLocation);
}

void ADragonsBreath::ModifyTerrain(const FVector& Location)
{
    // Get terrain data
    UWorld* World = GetWorld();
    if (!World) return;

    // Find terrain actors in radius
    TArray<AActor*> TerrainActors;
    UGameplayStatics::GetAllActorsInRadius(World, Location, TerrainModificationRadius, TerrainActors);

    for (AActor* TerrainActor : TerrainActors)
    {
        // Calculate distance-based strength
        float Distance = FVector::Distance(Location, TerrainActor->GetActorLocation());
        float DistanceFactor = FMath::Clamp(1.0f - (Distance / TerrainModificationRadius), 0.0f, 1.0f);
        float EffectStrength = TerrainModificationStrength * DistanceFactor * ElementalPower;

        // Apply terrain modification based on breath type
        switch (CurrentBreathType)
        {
            case EBreathType::CelestialFlame:
            {
                // Create scorched terrain with divine fire patterns
                if (ATerrainActor* Terrain = Cast<ATerrainActor>(TerrainActor))
                {
                    // Create radial scorch pattern
                    FVector Direction = (Terrain->GetActorLocation() - Location).GetSafeNormal();
                    float Angle = FMath::Atan2(Direction.Y, Direction.X);
                    
                    // Apply scorch effect with divine pattern
                    Terrain->ApplyScorchEffect(
                        EffectStrength,
                        FMath::Sin(Angle * 3.0f) * 0.5f + 0.5f, // Divine pattern
                        FLinearColor(1.0f, 0.8f, 0.2f, 1.0f)    // Celestial flame color
                    );
                    
                    // Spawn embers and divine fire particles
                    SpawnTerrainParticles(
                        Terrain->GetActorLocation(),
                        TEXT("/Game/Effects/Terrain/P_ScorchEmbers"),
                        EffectStrength
                    );
                }
                break;
            }
                
            case EBreathType::VoidFrost:
            {
                // Create frozen terrain with crystalline patterns
                if (ATerrainActor* Terrain = Cast<ATerrainActor>(TerrainActor))
                {
                    // Create crystalline frost pattern
                    FVector Direction = (Terrain->GetActorLocation() - Location).GetSafeNormal();
                    float Angle = FMath::Atan2(Direction.Y, Direction.X);
                    
                    // Apply frost effect with crystalline structure
                    Terrain->ApplyFrostEffect(
                        EffectStrength,
                        FMath::Sin(Angle * 6.0f) * 0.5f + 0.5f, // Crystalline pattern
                        FLinearColor(0.2f, 0.8f, 1.0f, 1.0f)    // Void frost color
                    );
                    
                    // Spawn frost crystals and mist
                    SpawnTerrainParticles(
                        Terrain->GetActorLocation(),
                        TEXT("/Game/Effects/Terrain/P_FrostCrystals"),
                        EffectStrength
                    );
                }
                break;
            }
                
            case EBreathType::StormWind:
            {
                // Create wind-swept terrain with dynamic patterns
                if (ATerrainActor* Terrain = Cast<ATerrainActor>(TerrainActor))
                {
                    // Create swirling wind pattern
                    FVector Direction = (Terrain->GetActorLocation() - Location).GetSafeNormal();
                    float Angle = FMath::Atan2(Direction.Y, Direction.X);
                    
                    // Apply wind erosion effect
                    Terrain->ApplyErosionEffect(
                        EffectStrength,
                        FMath::Sin(Angle * 4.0f + World->GetTimeSeconds()) * 0.5f + 0.5f, // Dynamic pattern
                        FLinearColor(0.7f, 0.7f, 1.0f, 0.8f)    // Storm wind color
                    );
                    
                    // Spawn wind particles and debris
                    SpawnTerrainParticles(
                        Terrain->GetActorLocation(),
                        TEXT("/Game/Effects/Terrain/P_WindDebris"),
                        EffectStrength
                    );
                }
                break;
            }
                
            case EBreathType::EarthForce:
            {
                // Create cracked terrain with seismic patterns
                if (ATerrainActor* Terrain = Cast<ATerrainActor>(TerrainActor))
                {
                    // Create seismic crack pattern
                    FVector Direction = (Terrain->GetActorLocation() - Location).GetSafeNormal();
                    float Angle = FMath::Atan2(Direction.Y, Direction.X);
                    
                    // Apply seismic effect
                    Terrain->ApplySeismicEffect(
                        EffectStrength,
                        FMath::Sin(Angle * 5.0f) * 0.5f + 0.5f, // Seismic pattern
                        FLinearColor(0.6f, 0.4f, 0.2f, 1.0f)    // Earth force color
                    );
                    
                    // Spawn dust and debris particles
                    SpawnTerrainParticles(
                        Terrain->GetActorLocation(),
                        TEXT("/Game/Effects/Terrain/P_EarthDebris"),
                        EffectStrength
                    );
                }
                break;
            }
        }
    }
}

void ADragonsBreath::SpawnTerrainParticles(const FVector& Location, const FString& ParticlePath, float Strength)
{
    if (UParticleSystem* ParticleSystem = Cast<UParticleSystem>(StaticLoadObject(
        UParticleSystem::StaticClass(),
        nullptr,
        *ParticlePath
    )))
    {
        // Spawn particles with scaled size and lifetime
        UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ParticleSystem,
            Location,
            FRotator::ZeroRotator,
            FVector(Strength),
            true,
            EPSCPoolMethod::AutoRelease
        );

        if (PSC)
        {
            // Scale particle properties based on effect strength
            PSC->SetFloatParameter(TEXT("ParticleSize"), Strength * 2.0f);
            PSC->SetFloatParameter(TEXT("ParticleLifetime"), Strength * 1.5f);
        }
    }
}

void ADragonsBreath::CreateEnvironmentalEffects(const FVector& Location)
{
    // Create environmental particle effects
    switch (CurrentBreathType)
    {
        case EBreathType::CelestialFlame:
            // Spawn fire environmental effects
            // TODO: Implement fire environmental effects
            break;
            
        case EBreathType::VoidFrost:
            // Spawn frost environmental effects
            // TODO: Implement frost environmental effects
            break;
            
        case EBreathType::StormWind:
            // Spawn wind environmental effects
            // TODO: Implement wind environmental effects
            break;
            
        case EBreathType::EarthForce:
            // Spawn earth environmental effects
            // TODO: Implement earth environmental effects
            break;
    }
}

float ADragonsBreath::CalculateEffectStrength(AActor* Target)
{
    if (!Target || !GetOwner()) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetOwner()->GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / BreathRange), 0.0f, 1.0f);

    // Apply breath type modifier
    float TypeModifier = 1.0f;
    switch (CurrentBreathType)
    {
        case EBreathType::CelestialFlame:
            TypeModifier = 1.2f;
            break;
        case EBreathType::VoidFrost:
            TypeModifier = 1.1f;
            break;
        case EBreathType::StormWind:
            TypeModifier = 0.9f;
            break;
        case EBreathType::EarthForce:
            TypeModifier = 1.4f;
            break;
    }

    return ElementalPower * DistanceFactor * TypeModifier;
}

void ADragonsBreath::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = ElementalPower * Modifier.Value * ResonanceGenerationRate;
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
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * ElementalPower);
        }
    }
} 