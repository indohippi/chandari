#include "Types/ResonanceTypes.h"

void FResonanceContainer::AddResonance(EResonanceType Type, float Amount)
{
    if (!Resonances.Contains(Type))
    {
        Resonances.Add(Type, FResonanceValue(Type));
    }
    
    FResonanceValue& ResonanceValue = Resonances[Type];
    ResonanceValue.Value = FMath::Clamp(ResonanceValue.Value + Amount, 0.0f, ResonanceValue.MaxValue);
}

void FResonanceContainer::RemoveResonance(EResonanceType Type, float Amount)
{
    if (Resonances.Contains(Type))
    {
        FResonanceValue& ResonanceValue = Resonances[Type];
        ResonanceValue.Value = FMath::Clamp(ResonanceValue.Value - Amount, 0.0f, ResonanceValue.MaxValue);
    }
}

float FResonanceContainer::GetResonanceValue(EResonanceType Type) const
{
    if (const FResonanceValue* ResonanceValue = Resonances.Find(Type))
    {
        return ResonanceValue->Value;
    }
    return 0.0f;
}

bool FResonanceContainer::HasResonance(EResonanceType Type) const
{
    return Resonances.Contains(Type) && Resonances[Type].Value > 0.0f;
}

void FResonanceContainer::ClearResonances()
{
    Resonances.Empty();
} 