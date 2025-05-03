#include "Types/EchoTypes.h"

void FEchoContainer::AddEcho(const FEchoEffect& Echo)
{
    // Check if we already have this type of echo
    for (FEchoEffect& ExistingEcho : ActiveEchoes)
    {
        if (ExistingEcho.Type == Echo.Type)
        {
            // Update existing echo
            ExistingEcho.Intensity = FMath::Max(ExistingEcho.Intensity, Echo.Intensity);
            ExistingEcho.Duration = FMath::Max(ExistingEcho.Duration, Echo.Duration);
            return;
        }
    }
    
    // Add new echo
    ActiveEchoes.Add(Echo);
}

void FEchoContainer::RemoveEcho(EEchoType Type)
{
    ActiveEchoes.RemoveAll([Type](const FEchoEffect& Echo) {
        return Echo.Type == Type;
    });
}

bool FEchoContainer::HasEcho(EEchoType Type) const
{
    return ActiveEchoes.ContainsByPredicate([Type](const FEchoEffect& Echo) {
        return Echo.Type == Type;
    });
}

void FEchoContainer::UpdateEchoes(float DeltaTime)
{
    for (int32 i = ActiveEchoes.Num() - 1; i >= 0; --i)
    {
        FEchoEffect& Echo = ActiveEchoes[i];
        
        if (Echo.Duration > 0.0f)
        {
            Echo.Duration -= DeltaTime;
            
            if (Echo.Duration <= 0.0f)
            {
                ActiveEchoes.RemoveAt(i);
            }
        }
    }
}

void FEchoContainer::ClearEchoes()
{
    ActiveEchoes.Empty();
} 