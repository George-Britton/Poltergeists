// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "Scares/ScareSpot.h"
#include "PlayerPoltergeist.generated.h"

class APlayerPoltergeist;

// Event delegate for the abilities
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnDash, APlayerPoltergeist, OnDash);

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

	// Event Dispatchers for abilities
	UPROPERTY(BlueprintAssignable, Category = "Abilities|Dash")
		FOnDash OnDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Dash")
		float DashSpeed = 2500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Dash")
		float DashTimeInSeconds = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|Dash")
		float DashCooldown = 5.f;
		float DashCooldownTimer = 0.f;
	
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
};