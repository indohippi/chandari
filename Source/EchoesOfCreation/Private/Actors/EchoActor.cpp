#include "EchoActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AEchoActor::AEchoActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and setup the sphere component
    EchoSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EchoSphere"));
    EchoSphere->InitSphereRadius(EchoRadius);
    EchoSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = EchoSphere;

    // Create and setup the particle system component
    EchoParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EchoParticles"));
    EchoParticles->SetupAttachment(RootComponent);
    EchoParticles->bAutoActivate = true;
}

void AEchoActor::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeEcho();
    OnEchoManifest();
}

void AEchoActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateEchoEffects();
}

void AEchoActor::UpdateEchoStrength(float DeltaStrength)
{
    EchoStrength = FMath::Clamp(EchoStrength + DeltaStrength, 0.0f, 1.0f);
    
    // Update visual effects based on new strength
    if (EchoParticles)
    {
        EchoParticles->SetFloatParameter(TEXT("Strength"), EchoStrength);
    }

    // Update sphere radius based on strength
    if (EchoSphere)
    {
        EchoSphere->SetSphereRadius(EchoRadius * EchoStrength);
    }
}

void AEchoActor::TriggerEchoEffect(AActor* TargetActor)
{
    if (TargetActor)
    {
        OnEchoInteract(TargetActor);
    }
}

void AEchoActor::InitializeEcho()
{
    // Set up initial particle system based on echo type
    switch (EchoType)
    {
        case EEchoType::Weaver:
            // Setup weaver-specific particles
            break;
        case EEchoType::Static:
            // Setup static-specific particles
            break;
        case EEchoType::Resonance:
            // Setup resonance-specific particles
            break;
        case EEchoType::Corrupted:
            // Setup corrupted-specific particles
            break;
    }

    // Set initial sphere radius
    if (EchoSphere)
    {
        EchoSphere->SetSphereRadius(EchoRadius * EchoStrength);
    }
}

void AEchoActor::UpdateEchoEffects()
{
    // Update particle effects based on current state
    if (EchoParticles)
    {
        // Update particle parameters based on echo type and strength
        switch (EchoType)
        {
            case EEchoType::Weaver:
                // Update weaver-specific effects
                break;
            case EEchoType::Static:
                // Update static-specific effects
                break;
            case EEchoType::Resonance:
                // Update resonance-specific effects
                break;
            case EEchoType::Corrupted:
                // Update corrupted-specific effects
 