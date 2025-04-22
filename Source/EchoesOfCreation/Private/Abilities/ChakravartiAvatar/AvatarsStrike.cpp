#include "Abilities/ChakravartiAvatar/AvatarsStrike.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ResonanceManager.h"
#include "EchoManager.h"
#include "Characters/EchoesCharacter.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DecalComponent.h"
#include "Components/PostProcessComponent.h"

AAvatarsStrike::AAvatarsStrike()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsActive = false;
    CurrentTarget = nullptr;
    StrikeRange = 1000.0f;

    // Set ability properties
    AbilityName = TEXT("Avatar's Strike");
    ResonanceCost = 40.0f;
    Cooldown = 25.0f;
    Range = 300.0f;
    SelectedType = EStrikeType::CosmicBlade;

    // Create post-process component for strike effects
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    RootComponent = PostProcessComponent;
}

void AAvatarsStrike::BeginPlay()
{
    Super::BeginPlay();

    // Initialize default values
    ResonanceGenerationRate = 2.0f;
    bIsActive = false;

    // Setup default resonance modifiers
    ResonanceModifiers.Add(EResonanceType::Faith, 2.5f);
    ResonanceModifiers[EResonanceType::Doubt] = 1.8f;
    ResonanceModifiers[EResonanceType::Curiosity] = 2.2f;

    // Setup default echo interactions
    EchoInteractions.Add(EEchoType::Divine, 2.5f);
    EchoInteractions.Add(EEchoType::Corrupted, 1.8f);
    EchoInteractions.Add(EEchoType::Warped, 2.2f);

    // Load visual effect assets
    if (!StrikeEffect)
    {
        StrikeEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/AvatarsStrike/NS_AvatarsStrike"));
    }
    if (!ImpactEffect)
    {
        ImpactEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/AvatarsStrike/NS_StrikeImpact"));
    }
    if (!TrailEffect)
    {
        TrailEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Effects/AvatarsStrike/NS_StrikeTrail"));
    }
    if (!StrikeDecal)
    {
        StrikeDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Effects/AvatarsStrike/M_StrikeDecal"));
    }
    if (!PostProcessMaterial)
    {
        PostProcessMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Effects/AvatarsStrike/M_StrikePostProcess"));
    }
}

void AAvatarsStrike::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        UpdateStrikeState(DeltaTime);
        if (CurrentTarget)
        {
            if (!CheckTargetValidity(CurrentTarget))
            {
                DeactivateStrike();
            }
        }
    }
}

void AAvatarsStrike::InitializeStrike(EStrikeType StrikeType, float BasePower, float Duration)
{
    CurrentStrikeType = StrikeType;
    StrikePower = BasePower;
    StrikeDuration = Duration;

    // Adjust properties based on strike type
    switch (StrikeType)
    {
        case EStrikeType::CosmicBlade:
            ResonanceModifiers[EResonanceType::Faith] = 3.0f;
            EchoInteractions[EEchoType::Divine] = 3.0f;
            break;
        case EStrikeType::DivineHammer:
            ResonanceModifiers[EResonanceType::Doubt] = 2.5f;
            EchoInteractions[EEchoType::Corrupted] = 2.5f;
            break;
        case EStrikeType::EternalSpear:
            ResonanceModifiers[EResonanceType::Curiosity] = 3.2f;
            EchoInteractions[EEchoType::Warped] = 3.0f;
            break;
    }
}

void AAvatarsStrike::ActivateStrike()
{
    if (!bIsActive)
    {
        bIsActive = true;
        OnStrikeActivated();
    }
}

void AAvatarsStrike::DeactivateStrike()
{
    if (bIsActive)
    {
        bIsActive = false;
        CurrentTarget = nullptr;
        OnStrikeDeactivated();
    }
}

void AAvatarsStrike::StrikeTarget(AActor* Target)
{
    if (!Target || !CheckTargetValidity(Target)) return;

    CurrentTarget = Target;
    float StrikeStrength = CalculateStrikeStrength(Target);
    ApplyStrikeEffects(Target);
    OnTargetStruck(Target, StrikeStrength);
}

void AAvatarsStrike::ApplyStrikeEffects(AActor* Target)
{
    if (!Target) return;

    if (ACharacter* Character = Cast<ACharacter>(Target))
    {
        float StrikeStrength = CalculateStrikeStrength(Target);

        switch (CurrentStrikeType)
        {
            case EStrikeType::CosmicBlade:
                // Apply Cosmic Blade effects (precision damage and faith resonance)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.9f; // Slight slow
                }
                // Apply precision damage
                UGameplayStatics::ApplyDamage(Target, StrikeStrength * 1.2f, nullptr, this, nullptr);

                // Spawn hit effect
                if (ImpactEffect)
                {
                    UNiagaraComponent* HitEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
                        ImpactEffect,
                        Character->GetRootComponent(),
                        NAME_None,
                        FVector::ZeroVector,
                        FRotator::ZeroRotator,
                        EAttachLocation::SnapToTarget,
                        true
                    );
                    if (HitEffect)
                    {
                        HitEffect->SetVariableFloat(FString("HitScale"), 0.5f);
                        HitEffect->SetVariableLinearColor(FString("HitColor"), FLinearColor(0.8f, 0.4f, 1.0f, 1.0f));
                    }
                }
                break;

            case EStrikeType::DivineHammer:
                // Apply Divine Hammer effects (area damage and doubt resonance)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.8f; // Moderate slow
                }
                // Apply area damage
                UGameplayStatics::ApplyDamage(Target, StrikeStrength * 1.5f, nullptr, this, nullptr);

                // Spawn hit effect
                if (ImpactEffect)
                {
                    UNiagaraComponent* HitEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
                        ImpactEffect,
                        Character->GetRootComponent(),
                        NAME_None,
                        FVector::ZeroVector,
                        FRotator::ZeroRotator,
                        EAttachLocation::SnapToTarget,
                        true
                    );
                    if (HitEffect)
                    {
                        HitEffect->SetVariableFloat(FString("HitScale"), 1.0f);
                        HitEffect->SetVariableLinearColor(FString("HitColor"), FLinearColor(1.0f, 0.8f, 0.2f, 1.0f));
                    }
                }
                break;

            case EStrikeType::EternalSpear:
                // Apply Eternal Spear effects (piercing damage and curiosity resonance)
                if (Character->GetCharacterMovement())
                {
                    Character->GetCharacterMovement()->MaxWalkSpeed *= 0.7f; // Significant slow
                }
                // Apply piercing damage
                UGameplayStatics::ApplyDamage(Target, StrikeStrength * 1.3f, nullptr, this, nullptr);

                // Spawn hit effect
                if (ImpactEffect)
                {
                    UNiagaraComponent* HitEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
                        ImpactEffect,
                        Character->GetRootComponent(),
                        NAME_None,
                        FVector::ZeroVector,
                        FRotator::ZeroRotator,
                        EAttachLocation::SnapToTarget,
                        true
                    );
                    if (HitEffect)
                    {
                        HitEffect->SetVariableFloat(FString("HitScale"), 0.3f);
                        HitEffect->SetVariableLinearColor(FString("HitColor"), FLinearColor(0.2f, 0.8f, 1.0f, 1.0f));
                    }
                }
                break;
        }
    }
}

float AAvatarsStrike::CalculateStrikeStrength(AActor* Target)
{
    if (!Target) return 0.0f;

    // Calculate base strength based on distance
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    float DistanceFactor = FMath::Clamp(1.0f - (Distance / StrikeRange), 0.0f, 1.0f);

    // Apply strike type modifier
    float TypeModifier = 1.0f;
    switch (CurrentStrikeType)
    {
        case EStrikeType::CosmicBlade:
            TypeModifier = 1.2f;
            break;
        case EStrikeType::DivineHammer:
            TypeModifier = 1.5f;
            break;
        case EStrikeType::EternalSpear:
            TypeModifier = 1.3f;
            break;
    }

    return StrikePower * DistanceFactor * TypeModifier;
}

void AAvatarsStrike::GenerateResonance()
{
    if (AResonanceManager* ResonanceManager = AResonanceManager::GetInstance())
    {
        for (const auto& Modifier : ResonanceModifiers)
        {
            float ResonanceAmount = StrikePower * Modifier.Value * ResonanceGenerationRate;
            ResonanceManager->AddResonance(Modifier.Key, ResonanceAmount);
        }
    }
}

void AAvatarsStrike::HandleEchoInteractions()
{
    if (AEchoManager* EchoManager = AEchoManager::GetInstance())
    {
        for (const auto& Interaction : EchoInteractions)
        {
            float EchoModifier = Interaction.Value;
            EchoManager->ModifyEchoStrength(Interaction.Key, EchoModifier * StrikePower);
        }
    }
}

void AAvatarsStrike::UpdateStrikeState(float DeltaTime)
{
    // Update strike duration
    StrikeDuration -= DeltaTime;
    if (StrikeDuration <= 0.0f)
    {
        DeactivateStrike();
        return;
    }

    // Generate resonance and handle echo interactions
    GenerateResonance();
    HandleEchoInteractions();

    // Apply effects to current target if valid
    if (CurrentTarget && CheckTargetValidity(CurrentTarget))
    {
        ApplyStrikeEffects(CurrentTarget);
    }
}

bool AAvatarsStrike::CheckTargetValidity(AActor* Target)
{
    if (!Target || Target->IsPendingKill()) return false;

    // Check if target is within range
    float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
    return Distance <= StrikeRange;
}

void AAvatarsStrike::SelectStrikeType(EStrikeType Type)
{
    SelectedType = Type;
}

bool AAvatarsStrike::ActivateAbility(const FVector& TargetLocation)
{
    if (!Super::ActivateAbility(TargetLocation))
    {
        return false;
    }

    // Create the strike effect
    CreateStrikeEffect(TargetLocation);

    // Apply effects to targets
    ApplyEffects(TargetLocation);

    // Generate resonance based on the selected strike type
    switch (SelectedType)
    {
        case EStrikeType::CosmicBlade:
            GenerateResonance(EResonanceType::Faith, 1.3f);
            break;
        case EStrikeType::DivineHammer:
            GenerateResonance(EResonanceType::Faith, 1.4f);
            break;
        case EStrikeType::EternalSpear:
            GenerateResonance(EResonanceType::Faith, 1.5f);
            break;
    }

    return true;
}

void AAvatarsStrike::CreateStrikeEffect(const FVector& TargetLocation)
{
    if (!GetOwner()) return;

    FVector StartLocation = GetOwner()->GetActorLocation();
    FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();
    FRotator StrikeRotation = Direction.Rotation();

    // Spawn main strike effect
    if (StrikeEffect)
    {
        UNiagaraComponent* StrikeComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            StrikeEffect,
            StartLocation,
            StrikeRotation
        );

        if (StrikeComponent)
        {
            // Set strike parameters based on type
            FLinearColor StrikeColor;
            float StrikeScale;
            switch (SelectedType)
            {
                case EStrikeType::CosmicBlade:
                    StrikeColor = FLinearColor(0.8f, 0.4f, 1.0f, 1.0f); // Purple
                    StrikeScale = 1.0f;
                    break;
                case EStrikeType::DivineHammer:
                    StrikeColor = FLinearColor(1.0f, 0.8f, 0.2f, 1.0f); // Golden
                    StrikeScale = 1.5f;
                    break;
                case EStrikeType::EternalSpear:
                    StrikeColor = FLinearColor(0.2f, 0.8f, 1.0f, 1.0f); // Blue
                    StrikeScale = 0.8f;
                    break;
            }

            StrikeComponent->SetVariableLinearColor(FString("StrikeColor"), StrikeColor);
            StrikeComponent->SetVariableFloat(FString("StrikeScale"), StrikeScale);
            StrikeComponent->SetVariableVec3(FString("TargetLocation"), TargetLocation);
        }
    }

    // Spawn trail effect
    if (TrailEffect)
    {
        UNiagaraComponent* TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            TrailEffect,
            StartLocation,
            StrikeRotation
        );

        if (TrailComponent)
        {
            // Set trail parameters based on type
            FLinearColor TrailColor;
            float TrailWidth;
            switch (SelectedType)
            {
                case EStrikeType::CosmicBlade:
                    TrailColor = FLinearColor(0.6f, 0.2f, 0.8f, 0.8f);
                    TrailWidth = 20.0f;
                    break;
                case EStrikeType::DivineHammer:
                    TrailColor = FLinearColor(0.8f, 0.6f, 0.1f, 0.8f);
                    TrailWidth = 40.0f;
                    break;
                case EStrikeType::EternalSpear:
                    TrailColor = FLinearColor(0.1f, 0.6f, 0.8f, 0.8f);
                    TrailWidth = 15.0f;
                    break;
            }

            TrailComponent->SetVariableLinearColor(FString("TrailColor"), TrailColor);
            TrailComponent->SetVariableFloat(FString("TrailWidth"), TrailWidth);
            TrailComponent->SetVariableVec3(FString("EndLocation"), TargetLocation);
        }
    }

    // Spawn impact effect at target location
    if (ImpactEffect)
    {
        UNiagaraComponent* ImpactComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ImpactEffect,
            TargetLocation,
            FRotator::ZeroRotator
        );

        if (ImpactComponent)
        {
            // Set impact parameters based on type
            float ImpactScale;
            float ImpactDuration;
            switch (SelectedType)
            {
                case EStrikeType::CosmicBlade:
                    ImpactScale = 1.2f;
                    ImpactDuration = 0.5f;
                    break;
                case EStrikeType::DivineHammer:
                    ImpactScale = 2.0f;
                    ImpactDuration = 0.8f;
                    break;
                case EStrikeType::EternalSpear:
                    ImpactScale = 1.0f;
                    ImpactDuration = 0.3f;
                    break;
            }

            ImpactComponent->SetVariableFloat(FString("ImpactScale"), ImpactScale);
            ImpactComponent->SetVariableFloat(FString("Duration"), ImpactDuration);
        }
    }

    // Spawn decal at impact point
    if (StrikeDecal)
    {
        FRotator DecalRotation = FRotator(-90.0f, 0.0f, 0.0f);
        UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
            GetWorld(),
            StrikeDecal,
            FVector(200.0f, 200.0f, 200.0f),
            TargetLocation,
            DecalRotation,
            5.0f
        );

        if (DecalComponent)
        {
            // Create dynamic material instance for decal
            UMaterialInstanceDynamic* DecalMaterial = UMaterialInstanceDynamic::Create(StrikeDecal, this);
            if (DecalMaterial)
            {
                // Set decal parameters based on type
                FLinearColor DecalColor;
                float DecalIntensity;
                switch (SelectedType)
                {
                    case EStrikeType::CosmicBlade:
                        DecalColor = FLinearColor(0.8f, 0.4f, 1.0f, 0.8f);
                        DecalIntensity = 2.0f;
                        break;
                    case EStrikeType::DivineHammer:
                        DecalColor = FLinearColor(1.0f, 0.8f, 0.2f, 0.8f);
                        DecalIntensity = 3.0f;
                        break;
                    case EStrikeType::EternalSpear:
                        DecalColor = FLinearColor(0.2f, 0.8f, 1.0f, 0.8f);
                        DecalIntensity = 1.5f;
                        break;
                }

                DecalMaterial->SetVectorParameterValue(FName("Color"), DecalColor);
                DecalMaterial->SetScalarParameterValue(FName("Intensity"), DecalIntensity);
                DecalComponent->SetMaterial(0, DecalMaterial);
            }
        }
    }

    // Apply post-process effect
    if (PostProcessMaterial)
    {
        UMaterialInstanceDynamic* PostProcessInstance = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
        if (PostProcessInstance && PostProcessComponent)
        {
            // Set post-process parameters based on type
            float DistortionIntensity;
            float ChromaticAberration;
            switch (SelectedType)
            {
                case EStrikeType::CosmicBlade:
                    DistortionIntensity = 0.3f;
                    ChromaticAberration = 0.2f;
                    break;
                case EStrikeType::DivineHammer:
                    DistortionIntensity = 0.5f;
                    ChromaticAberration = 0.4f;
                    break;
                case EStrikeType::EternalSpear:
                    DistortionIntensity = 0.2f;
                    ChromaticAberration = 0.1f;
                    break;
            }

            PostProcessInstance->SetScalarParameterValue(FName("DistortionIntensity"), DistortionIntensity);
            PostProcessInstance->SetScalarParameterValue(FName("ChromaticAberration"), ChromaticAberration);

            PostProcessComponent->Settings.WeightedBlendables.Array.Empty();
            PostProcessComponent->Settings.WeightedBlendables.Array.Add(
                FWeightedBlendable(1.0f, PostProcessInstance)
            );

            // Set timer to remove post-process effect
            FTimerHandle PostProcessTimer;
            GetWorld()->GetTimerManager().SetTimer(
                PostProcessTimer,
                [this]()
                {
                    if (PostProcessComponent)
                    {
                        PostProcessComponent->Settings.WeightedBlendables.Array.Empty();
                    }
                },
                0.5f,
                false
            );
        }
    }
}

void AAvatarsStrike::ApplyEffects(const FVector& TargetLocation)
{
    // Find all characters in the strike radius
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEchoesCharacter::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AEchoesCharacter* Character = Cast<AEchoesCharacter>(Actor);
        if (Character)
        {
            float Distance = FVector::Distance(TargetLocation, Character->GetActorLocation());
            if (Distance <= Range)
            {
                ApplyStrikeEffects(Character);
            }
        }
    }
} 