#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Echoes/EchoTypes.h"
#include "Resonance/ResonanceTypes.h"
#include "EchoesCharacter.generated.h"

// Base character archetypes
UENUM(BlueprintType)
enum class EBaseArchetype : uint8
{
    Weaver,         // Manipulates cosmic threads and connections
    Scholar,        // Seeks and wields ancient knowledge
    Warrior,        // Embodies divine combat prowess
    Mystic,         // Channels spiritual and mystical forces
    Guardian,       // Protects and maintains cosmic balance
    Trickster,      // Uses deception and cunning
    Sage,           // Masters wisdom and enlightenment
    Herald,         // Communicates with divine entities
    Architect,      // Shapes and reconstructs reality
    Wanderer        // Explores and discovers new paths
};

// Subclass specializations
UENUM(BlueprintType)
enum class ESubclass : uint8
{
    // Weaver subclasses
    ThreadBinder,   // Binds and weaves cosmic threads
    PatternSeeker,  // Discovers hidden patterns
    FateSpinner,    // Influences destiny
    WebWalker,      // Navigates cosmic connections
    TapestryMender, // Repairs broken realities

    // Scholar subclasses
    LoreKeeper,     // Preserves ancient knowledge
    TruthSeeker,    // Uncovers hidden truths
    CodeBreaker,    // Deciphers cosmic mysteries
    MemoryWalker,   // Traverses collective memory
    KnowledgeForge, // Creates new understanding

    // Warrior subclasses
    BladeDancer,    // Masters divine combat
    ShieldBearer,   // Protects cosmic order
    StormCaller,    // Wields elemental forces
    PathWalker,     // Follows warrior's path
    HonorGuard,     // Upholds divine justice

    // Mystic subclasses
    DreamWalker,    // Navigates dream realms
    SpiritChannel,  // Connects with spirits
    RuneMaster,     // Wields ancient symbols
    VisionSeeker,   // Sees beyond reality
    EssenceBinder,  // Binds spiritual energy

    // Guardian subclasses
    BalanceKeeper,  // Maintains cosmic balance
    GateWarden,     // Guards dimensional gates
    TimeWatcher,    // Oversees temporal flow
    SpaceGuardian,  // Protects spatial integrity
    RealityWarden,  // Guards against corruption

    // Trickster subclasses
    ShadowDancer,   // Masters deception
    ChaosWeaver,    // Creates controlled chaos
    Illusionist,    // Manipulates perception
    FateTwister,    // Alters destiny
    RealityJester,  // Challenges cosmic order

    // Sage subclasses
    WisdomBearer,   // Carries ancient wisdom
    EnlightenmentSeeker, // Seeks cosmic truth
    HarmonyMaster,  // Masters balance
    KnowledgeFlow,  // Channels understanding
    TruthSpeaker,   // Speaks divine truth

    // Herald subclasses
    DivineMessenger, // Carries divine messages
    Oracle,         // Sees future events
    Prophet,        // Foretells destiny
    VoiceOfGods,    // Speaks for deities
    CosmicHerald,   // Announces cosmic events

    // Architect subclasses
    RealityShaper,  // Shapes physical reality
    DreamBuilder,   // Creates dream realms
    SpaceForge,     // Forges new spaces
    TimeArchitect,  // Designs temporal flows
    CosmicDesigner, // Designs cosmic order

    // Wanderer subclasses
    Pathfinder,     // Discovers new paths
    RealityWalker,  // Walks between realities
    EchoHunter,     // Seeks cosmic echoes
    DimensionRover, // Explores dimensions
    CosmicExplorer  // Explores the cosmos
};

UCLASS()
class ECHOESOFCREATION_API AEchoesCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AEchoesCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Grid-based movement
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool MoveToGridPosition(const FVector& GridPosition);

    // Combat actions
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool UseAbility(int32 AbilityIndex, const FVector& TargetPosition);

    // Resonance management
    UFUNCTION(BlueprintCallable, Category = "Resonance")
    void GenerateResonance(EResonanceType Type, float Intensity = 1.0f);

    // Character progression
    UFUNCTION(BlueprintCallable, Category = "Character")
    void LevelUp();

    UFUNCTION(BlueprintCallable, Category = "Character")
    void EvolveSubclass(ESubclass NewSubclass);

    // Character stats and progression
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    EBaseArchetype BaseArchetype;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    ESubclass CurrentSubclass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    float ResonanceCapacity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    float CurrentResonance = 0.0f;

    // Movement and action points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    int32 MovementRange = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    int32 ActionPoints = 2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    int32 RemainingActionPoints;

    // Team and positioning
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    int32 TeamID = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
    TArray<AEchoesCharacter*> TeamMembers;

protected:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UResonanceComponent* ResonanceComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UEchoComponent* EchoComponent;

    // Grid position
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    FVector CurrentGridPosition;

    // Movement and combat functions
    UFUNCTION()
    void OnMovementComplete();

    UFUNCTION()
    void OnAbilityComplete();

    // Resonance generation cooldown
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance")
    float ResonanceCooldown = 1.0f;

    // Last resonance generation time
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resonance")
    float LastResonanceTime;
}; 