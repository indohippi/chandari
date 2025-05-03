#include "Characters/TestCharacter.h"

ATestCharacter::ATestCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ATestCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Add some test resonances
    TestAddResonance(EResonanceType::Creation, 50.0f);
    TestAddResonance(EResonanceType::Fire, 30.0f);
    
    // Add a test echo
    TestAddEcho(EEchoType::CreationEcho, 1.0f, 5.0f);
}

void ATestCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateEchoes(DeltaTime);
}

void ATestCharacter::TestAddResonance(EResonanceType Type, float Amount)
{
    ResonanceContainer.AddResonance(Type, Amount);
    UE_LOG(LogTemp, Log, TEXT("Added Resonance - Type: %d, Amount: %f"), (int32)Type, Amount);
}

void ATestCharacter::TestAddEcho(EEchoType Type, float Intensity, float Duration)
{
    FEchoEffect NewEcho(Type, Intensity, Duration);
    EchoContainer.AddEcho(NewEcho);
    UE_LOG(LogTemp, Log, TEXT("Added Echo - Type: %d, Intensity: %f, Duration: %f"), (int32)Type, Intensity, Duration);
}

float ATestCharacter::GetCurrentResonance(EResonanceType Type) const
{
    return ResonanceContainer.GetResonanceValue(Type);
}

bool ATestCharacter::HasActiveEcho(EEchoType Type) const
{
    return EchoContainer.HasEcho(Type);
}

void ATestCharacter::UpdateEchoes(float DeltaTime)
{
    EchoContainer.UpdateEchoes(DeltaTime);
} 