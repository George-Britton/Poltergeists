// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "ScareSpot.generated.h"

// Enum for the effect type of the scare spot
UENUM()
enum class EScareType : uint8
{
	VISUAL UMETA(DisplayName="Visual"),
	AUDIO UMETA(DisplayName="Audio"),
	PHYSICAL UMETA(DisplayName="Physical"),
	MAX
};

// Enum for the activation state of the scare spot
UENUM()
enum class EScareState : uint8
{
	READY UMETA(DisplayName="Ready"),
	ACTIVE UMETA(DisplayName="Active"),
	COOLDOWN UMETA(DisplayName="Cooldown"),
	OFF UMETA(DisplayName="Off"),
	MAX
};

UCLASS()
class POLTERGEISTS_API AScareSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScareSpot();

	// The main mesh of the scare spot#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Apperance")
		UStaticMesh* ScareSpotMesh;
	UPROPERTY()
		UStaticMeshComponent* ScareSpotMeshComponent;
	
	// Distance the player must be within to activate the scare spot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		float ActivationDistance = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		FVector ActivationSphereRelativeLocation = FVector(0, 0, 0);
	UPROPERTY()
		USphereComponent* ActivationSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		bool Retriggerable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		int32 RequiredPlayersToActivate = 1;
	
	// Time for which the scare spot is in use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		float ActiveTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		float CooldownTime = 3.f;
	UPROPERTY()
		float ActiveTimer = 0.f;
	UPROPERTY()
		float CooldownTimer = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Activation")
		EScareState ScareState = EScareState::READY;

	// Details about the specific scare
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare")
		float ScareStrength = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scare")
		EScareType ScareType = EScareType::MAX;

protected:
	// Called whenever a value is changed
	void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the player activates the scare spot
	UFUNCTION(BlueprintCallable, Category = "Activation", DisplayName = "ActivateScareSpot")
		void ReceiveActivateScareSpot();
	UFUNCTION(BlueprintImplementableEvent, Category = "Activation")
		void ActivateScareSpot();

	// Called when the scare spot finishes scaring and starts the cooldown period
	UFUNCTION()
		void BeginCooldown();

	// Called when the scare spot is made active again
	UFUNCTION()
		void ResetScareSpot();

	// Called when the state changes
	UFUNCTION(BlueprintNativeEvent, Category = "Cooldown")
		void OnScareFinish();
		void OnScareFinish_Implementation(){};
	UFUNCTION(BlueprintNativeEvent, Category = "Activation")
		void OnScareReset();
		void OnScareReset_Implementation(){};
};
