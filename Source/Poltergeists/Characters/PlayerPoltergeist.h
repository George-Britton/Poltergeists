// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/SpecialAbilityComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Scares/ScareSpot.h"
#include "Camera/CameraActor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerPoltergeist.generated.h"


// Enum for the inter-room chasing
UENUM(BlueprintType)
enum class EChaseState : uint8
{
	FADING_IN UMETA(DisplayName="Fading In"),
	FADING_OUT UMETA(DisplayName="Fading Out"),
	MOVING UMETA(DisplayName="Moving"),
	WAITING UMETA(DisplayName="Waiting"),
	PLAYING UMETA(DisplayName="Playing"),
	MAX
};

// Enum for the player ability types
UENUM()
enum class EPlayerAbility : uint8
{
	TOUCHE UMETA(DisplayName="Touche"),
	CURSE UMETA(DisplayName="Curse"),
	TIMEBOMB UMETA(DisplayName="Time Bomb"),
	TRAP UMETA(DisplayName="Trap"),
	MAX
};

// Event delegate for the abilities
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnDash, APlayerPoltergeist, OnDash);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnYeet, APlayerPoltergeist, OnYeet);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnSpecial, APlayerPoltergeist, OnSpecial);

UCLASS()
class POLTERGEISTS_API APlayerPoltergeist : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPoltergeist();
	
	// Camera for this player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		ACameraActor* PlayerCamera;

	// ID of the player, used for specific actor and location assigning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 PlayerID = 0;
	
	// The victim
	UPROPERTY(BlueprintReadOnly, Category = Victim)
		class AVictim* Victim;
	
	// Overlapped scare spot
	UPROPERTY(BlueprintReadOnly, Category = "Scares")
		class AScareSpot* OverlappedScareSpot;

	// Time it takes for the player's scare ability to cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scares")
		float Cooldown = 3.f;
	UPROPERTY(BlueprintReadOnly, Category = "Scares")
		float CooldownTimer = 0.f;

	// The sounds played when the player uses an ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
		USoundBase* ScareSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
		USoundBase* YeetSound;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
		USoundBase* DashSound;
	UPROPERTY()
		UAudioComponent* AbilitySpeaker;
	
	// Event Dispatchers and abilities variables
	// Dash
	UPROPERTY(BlueprintAssignable, Category = "Abilities|Dash")
		FOnDash OnDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Dash")
		float DashSpeed = 2500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Dash")
		float DashTimeInSeconds = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Dash")
		float DashCooldown = 5.f;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Dash")
		float DashCooldownTimer = 0.f;
	// Yeet
	UPROPERTY(BlueprintAssignable, Category = "Abilities|Yeet")
		FOnYeet OnYeet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Yeet")
		float PickupDistance = 125.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Yeet")
		float YeetStrength = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Yeet")
		float YeetIncline = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Yeet")
		float YeetCooldown = 5.f;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		float YeetCooldownTimer = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Abilities|Yeet")
		UPhysicsHandleComponent* PhysicsHandle;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		bool IsItemHeld = false;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		UPrimitiveComponent* ComponentHeld;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		class AYeetable* ObjectBeingHeld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Yeet")
		float ItemHeldDistance = 50.f;
	// Special
	UPROPERTY(BlueprintAssignable, Category = "Abilities|Special")
		FOnSpecial OnSpecial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		class USpecialAbilityComponent* SpecialComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		EPlayerAbility SpecialAbility = EPlayerAbility::TOUCHE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		float SpecialCooldown = 10.f;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Special")
		float SpecialCooldownTimer = 0.f;
	UPROPERTY()
		class AVictim* OverlappingVictim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Abilities|Special", meta = (EditCondition = "SpecialAbility == EPlayerAbility::TRAP"))
		UStaticMesh* TrapMesh;
	// The special ability sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		USoundBase* ToucheSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		USoundBase* CurseSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		USoundBase* TimeBombSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		USoundBase* TrapSound;
	
	
	// Used to chase the AI to the next room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
		EChaseState ChaseState = EChaseState::PLAYING;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rounds")
		FVector RunToLocation = FVector::ZeroVector;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to intialise the controller and player assignment
	UFUNCTION(BlueprintCallable, Category = "Control", DisplayName = "Initialise")
		void ReceiveInitialise(int32 ID, int32 SpecialID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Control")
		void Initialise(int32 ID, int32 SpecialID);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called whenever the player overlaps with something or stops overlapping
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Input axis for movement
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	// Input action for scare activation
	UFUNCTION(BlueprintCallable, Category = "Activation", DisplayName = "ActivateScareSpot")
		void ReceiveActivateScare();
	UFUNCTION(BlueprintImplementableEvent, Category = "Activation")
		void ActivateScare();

	// Input actions for abilities
	void Dash();
	void Pickup();
	void Yeet();
	void Special();
	void DeclareSpecialDone();

	// Called when the AI runs away
	UFUNCTION()
		void OnRunAway();
	// Called when the AI starts the next round
	UFUNCTION()
		void OnRoundStart();
};