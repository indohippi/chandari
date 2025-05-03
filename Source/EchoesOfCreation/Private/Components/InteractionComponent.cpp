#include "InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Components/ResonanceComponent.h"
#include "Actors/EchoActor.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsInteracting = false;
    CurrentInteractionProgress = 0.0f;
    CurrentInteractionTarget = nullptr;
    CurrentState = EInteractionState::Idle;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsInteracting)
    {
        UpdateInteractionProgress(DeltaTime);
        UpdateResonanceEffects(DeltaTime);
    }
}

void UInteractionComponent::StartInteraction(AActor* TargetActor)
{
    if (!TargetActor || bIsInteracting)
    {
        return;
    }

    FInteractionData InteractionData = GetInteractionData(TargetActor);
    
    if (!CanInteractWith(TargetActor))
    {
        HandleInteractionStateChange(EInteractionState::Failed);
        OnInteractionFailed(TargetActor, "Cannot interact with target");
        return;
    }

    CurrentInteractionTarget = TargetActor;
    bIsInteracting = true;
    CurrentInteractionProgress = 0.0f;
    HandleInteractionStateChange(EInteractionState::Starting);

    OnInteractionStarted(TargetActor, InteractionData);
}

void UInteractionComponent::StopInteraction()
{
    if (bIsInteracting)
    {
        CancelInteraction();
    }
}

bool UInteractionComponent::CanInteractWith(AActor* TargetActor) const
{
    if (!TargetActor)
    {
        return false;
    }

    // Check distance
    float Distance = FVector::Distance(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
    if (Distance > InteractionRange)
    {
        return false;
    }

    // Check resonance requirements
    FInteractionData InteractionData = GetInteractionData(TargetActor);
    if (InteractionData.bRequiresResonance)
    {
        UResonanceComponent* ResonanceComponent = GetOwner()->FindComponentByClass<UResonanceComponent>();
        if (!ResonanceComponent)
        {
            return false;
        }

        return ResonanceComponent->GetResonance(InteractionData.RequiredResonanceType) >= InteractionData.RequiredResonanceAmount;
    }

    return true;
}

void UInteractionComponent::ApplyResonanceToTarget(AActor* TargetActor, EResonanceType Type, float Amount)
{
    if (!TargetActor)
    {
        return;
    }

    UResonanceComponent* TargetResonance = TargetActor->FindComponentByClass<UResonanceComponent>();
    if (TargetResonance)
    {
        TargetResonance->AddResonance(Type, Amount);
    }
}

void UInteractionComponent::UpdateResonanceEffects(float DeltaTime)
{
    if (!CurrentInteractionTarget)
    {
        return;
    }

    FInteractionData InteractionData = GetInteractionData(CurrentInteractionTarget);
    if (InteractionData.ResonanceGain > 0.0f)
    {
        for (EResonanceType Type : InteractionData.AffectedResonanceTypes)
        {
            ApplyResonanceToTarget(CurrentInteractionTarget, Type, InteractionData.ResonanceGain * DeltaTime);
        }
    }
}

void UInteractionComponent::ProcessEchoInteraction(AActor* EchoActor)
{
    if (AEchoActor* Echo = Cast<AEchoActor>(EchoActor))
    {
        FInteractionData InteractionData = GetInteractionData(EchoActor);
        HandleEchoResonance(EchoActor, InteractionData.ResonanceGain);
        ApplyResonanceEffects(InteractionData);
    }
}

void UInteractionComponent::HandleEchoResonance(AActor* EchoActor, float ResonanceAmount)
{
    if (AEchoActor* Echo = Cast<AEchoActor>(EchoActor))
    {
        // Apply resonance effects specific to echoes
        for (EResonanceType Type : Echo->GetAffectedResonanceTypes())
        {
            ApplyResonanceToTarget(EchoActor, Type, ResonanceAmount);
        }
    }
}

void UInteractionComponent::UpdateInteractionProgress(float DeltaTime)
{
    if (!CurrentInteractionTarget)
    {
        CancelInteraction();
        return;
    }

    CurrentInteractionProgress += DeltaTime / InteractionTime;
    OnInteractionProgress(CurrentInteractionTarget, CurrentInteractionProgress);

    if (CurrentInteractionProgress >= 1.0f)
    {
        HandleInteractionStateChange(EInteractionState::Completing);
        CompleteInteraction();
    }
}

void UInteractionComponent::CompleteInteraction()
{
    if (CurrentInteractionTarget)
    {
        FInteractionData InteractionData = GetInteractionData(CurrentInteractionTarget);
        ApplyResonanceEffects(InteractionData);
        OnInteractionCompleted(CurrentInteractionTarget, InteractionData);
    }

    bIsInteracting = false;
    CurrentInteractionProgress = 0.0f;
    CurrentInteractionTarget = nullptr;
    HandleInteractionStateChange(EInteractionState::Idle);
}

void UInteractionComponent::CancelInteraction()
{
    if (CurrentInteractionTarget)
    {
        OnInteractionFailed(CurrentInteractionTarget, "Interaction cancelled");
    }

    bIsInteracting = false;
    CurrentInteractionProgress = 0.0f;
    CurrentInteractionTarget = nullptr;
    HandleInteractionStateChange(EInteractionState::Idle);
}

void UInteractionComponent::HandleInteractionStateChange(EInteractionState NewState)
{
    CurrentState = NewState;
    // Additional state change handling can be added here
}

FInteractionData UInteractionComponent::GetInteractionData(AActor* TargetActor) const
{
    FInteractionData Data;
    
    if (AEchoActor* EchoActor = Cast<AEchoActor>(TargetActor))
    {
        Data.Type = EInteractionType::Echo;
        Data.InteractionText = "Interact with Echo";
        Data.bRequiresResonance = true;
        Data.RequiredResonanceType = EResonanceType::Curiosity;
        Data.RequiredResonanceAmount = 10.0f;
        Data.ResonanceGain = 5.0f;
        Data.AffectedResonanceTypes = { EResonanceType::Faith, EResonanceType::Doubt, EResonanceType::Curiosity };
    }
    else
    {
        Data.Type = EInteractionType::Object;
        Data.InteractionText = "Interact with Object";
        Data.bRequiresResonance = false;
        Data.ResonanceGain = 1.0f;
        Data.AffectedResonanceTypes = { EResonanceType::Curiosity };
    }

    return Data;
}

void UInteractionComponent::ApplyResonanceEffects(const FInteractionData& InteractionData)
{
    if (CurrentInteractionTarget)
    {
        for (EResonanceType Type : InteractionData.AffectedResonanceTypes)
        {
            ApplyResonanceToTarget(CurrentInteractionTarget, Type, InteractionData.ResonanceGain);
        }
    }
} 