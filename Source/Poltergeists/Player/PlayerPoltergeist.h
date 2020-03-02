// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "Scares/ScareSpot.h"
#include "PlayerPoltergeist.generated.h"

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
	void ActivateScareSpot();
};