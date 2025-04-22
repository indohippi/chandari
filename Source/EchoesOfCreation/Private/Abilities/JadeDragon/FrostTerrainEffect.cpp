#include "FrostTerrainEffect.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UFrostTerrainEffect::UFrostTerrainEffect()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.1f;

    FrostRadius = 200.0f;
    FrostDuration = 5.0f;
    FrostIntensity = 1.0f;
    FrostDecayRate = 0.2f;
}

void UFrostTerrainEffect::BeginPlay()
{
    Super::BeginPlay();
    LoadEffectAssets();
}

void UFrostTerrainEffect::LoadEffectAssets()
{
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FrostTerrainEffectAsset(TEXT("/Game/Particles/Dragons/NS_FrostTerrain"));
    if (FrostTerrainEffectAsset.Succeeded())
    {
        FrostTerrainEffect = FrostTerrainEffectAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> FrostDecalMaterialAsset(TEXT("/Game/Materials/Dragons/M_FrostDecal"));
    if (FrostDecalMaterialAsset.Succeeded())
    {
        FrostDecalMaterial = FrostDecalMaterialAsset.Object;
    }
}

void UFrostTerrainEffect::ApplyFrostEffect(const FVector& Location, float Intensity)
{
    if (!FrostTerrainEffect || !FrostDecalMaterial)
    {
        return;
    }

    // Spawn frost terrain effect
    UNiagaraComponent* FrostEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        FrostTerrainEffect,
        Location,
        FRotator::ZeroRotator,
        FVector(1.0f),
        true,
        true,
        ENCPoolMethod::AutoRelease
    );

    if (FrostEffect)
    {
        FrostEffect->SetFloatParameter("Intensity", Intensity);
        FrostEffect->SetFloatParameter("Radius", FrostRadius);
        ActiveEffects.Add(FrostEffect);
    }

    // Spawn frost decal
    UDecalComponent* FrostDecal = UGameplayStatics::SpawnDecalAtLocation(
        GetWorld(),
        FrostDecalMaterial,
        FVector(FrostRadius * 2.0f),
        Location,
        FRotator::ZeroRotator,
        FrostDuration
    );

    if (FrostDecal)
    {
        FrostDecal->SetFadeScreenSize(0.0f);
        ActiveDecals.Add(FrostDecal);
    }

    // Apply physical material changes
    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    if (GetWorld()->SweepMultiByChannel(
        HitResults,
        Location,
        Location,
        FQuat::Identity,
        ECC_WorldStatic,
        FCollisionShape::MakeSphere(FrostRadius),
        QueryParams
    ))
    {
        for (const FHitResult& Hit : HitResults)
        {
            if (Hit.Component.IsValid())
            {
                UPhysicalMaterial* PhysMat = Hit.Component->GetBodyInstance()->GetSimplePhysicalMaterial();
                if (PhysMat)
                {
                    OriginalFriction.Add(PhysMat, PhysMat->Friction);
                    PhysMat->Friction *= 0.3f; // Reduce friction for slippery surface
                }
            }
        }
    }

    // Start decay timer
    GetWorld()->GetTimerManager().SetTimer(
        FrostDecayTimer,
        this,
        &UFrostTerrainEffect::DecayFrostEffect,
        FrostDuration,
        false
    );
}

void UFrostTerrainEffect::DecayFrostEffect()
{
    // Gradually reduce effect intensity
    for (UNiagaraComponent* Effect : ActiveEffects)
    {
        if (Effect && Effect->IsValidLowLevel())
        {
            float CurrentIntensity = Effect->GetFloatParameter("Intensity");
            Effect->SetFloatParameter("Intensity", CurrentIntensity - FrostDecayRate);
        }
    }

    // Remove fully decayed effects
    ActiveEffects.RemoveAll([](UNiagaraComponent* Effect) {
        return !Effect || !Effect->IsValidLowLevel() || Effect->GetFloatParameter("Intensity") <= 0.0f;
    });

    // Remove decayed decals
    ActiveDecals.RemoveAll([](UDecalComponent* Decal) {
        return !Decal || !Decal->IsValidLowLevel() || Decal->GetFadeOutDuration() <= 0.0f;
    });

    // Restore original friction values
    for (const auto& Pair : OriginalFriction)
    {
        if (Pair.Key && Pair.Key->IsValidLowLevel())
        {
            Pair.Key->Friction = Pair.Value;
        }
    }
    OriginalFriction.Empty();
}

void UFrostTerrainEffect::TickComponent(float DeltaTime, PIE_Game_Properties)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update effect parameters based on game state
    for (UNiagaraComponent* Effect : ActiveEffects)
    {
        if (Effect && Effect->IsValidLowLevel())
        {
            // Update effect parameters based on time and intensity
            float CurrentIntensity = Effect->GetFloatParameter("Intensity");
            Effect->SetFloatParameter("Time", GetWorld()->GetTimeSeconds());
            Effect->SetFloatParameter("Intensity", CurrentIntensity);
        }
    }
}

void UFrostTerrainEffect::Cleanup()
{
    // Clean up all active effects
    for (UNiagaraComponent* Effect : ActiveEffects)
    {
        if (Effect && Effect->IsValidLowLevel())
        {
            Effect->DestroyComponent();
        }
    }
    ActiveEffects.Empty();

    // Clean up all active decals
    for (UDecalComponent* Decal : ActiveDecals)
    {
        if (Decal && Decal->IsValidLowLevel())
        {
            Decal->DestroyComponent();
        }
    }
    ActiveDecals.Empty();

    // Restore original friction values
    for (const auto& Pair : OriginalFriction)
    {
        if (Pair.Key && Pair.Key->IsValidLowLevel())
        {
            Pair.Key->Friction = Pair.Value;
        }
    }
    OriginalFriction.Empty();
} 