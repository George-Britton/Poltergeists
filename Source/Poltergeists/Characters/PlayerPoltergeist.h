// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "Scares/ScareSpot.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Abilities/Yeetable.h"
#include "PlayerPoltergeist.generated.h"

class APlayerPoltergeist;

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

	// Overlapped scare spot
	UPROPERTY(BlueprintReadOnly, Category = "Scares")
		AScareSpot* OverlappedScareSpot;

	// Time it takes for the player's scare ability to cooldown
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scares")
		float Cooldown = 3.f;
	UPROPERTY()
		float CooldownTimer = 0.f;

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
		float YeetCooldownTimer = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Abilities|Yeet")
		UPhysicsHandleComponent* PhysicsHandle;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		bool IsItemHeld = false;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		UPrimitiveComponent* ComponentHeld;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities|Yeet")
		AYeetable* ObjectBeingHeld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Yeet")
		float ItemHeldDistance = 50.f;
	// Special
	UPROPERTY(BlueprintAssignable, Category = "Abilities|Special")
		FOnSpecial OnSpecial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		EPlayerAbility SpecialAbility = EPlayerAbility::MAX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special")
		float SpecialCooldown = 10.f;
		float SpecialCooldownTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|Curse", meta = (EditCondition="SpecialAbility == EPlayerAbility::CURSE"))
		float CurseMultiplier = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|Curse", meta = (EditCondition = "SpecialAbility == EPlayerAbility::CURSE"))
		float CurseTime = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|Touche", meta = (EditCondition = "SpecialAbility == EPlayerAbility::TOUCHE"))
		float ToucheStrength = 80.f;
		AVictim* OverlappingVictim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|TimeBomb", meta = (EditCondition = "SpecialAbility == EPlayerAbility::TIMEBOMB"))
		float TimeBombDelay = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|Trap", meta = (EditCondition = "SpecialAbility == EPlayerAbility::TRAP"))
		float TrapStrength = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|Trap", meta = (EditCondition = "SpecialAbility == EPlayerAbility::TRAP"))
		float TrapTime = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Special|Trap", meta = (EditCondition = "SpecialAbility == EPlayerAbility::TRAP"))
		UStaticMesh* TrapMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
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
	void Touche();
	void Trap();
	void TimeBomb();
	void Curse();
	void DeclareSpecialDone();
};