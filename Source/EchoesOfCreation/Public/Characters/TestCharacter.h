#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/ResonanceTypes.h"
#include "Types/EchoTypes.h"
#include "TestCharacter.generated.h"

UCLASS()
class ECHOESOFCREATION_API ATestCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATestCharacter();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    // Test functions
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestAddResonance(EResonanceType Type, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestAddEcho(EEchoType Type, float Intensity, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Test")
    float GetCurrentResonance(EResonanceType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Test")
    bool HasActiveEcho(EEchoType Type) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    FResonanceContainer ResonanceContainer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo")
    FEchoContainer EchoContainer;

private:
    void UpdateEchoes(float DeltaTime);
}; 