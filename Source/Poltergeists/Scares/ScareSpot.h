// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "ScareSpot.generated.h"

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
	UPROPERTY()
		USphereComponent* ActivationSphere;

	// Time for which the scare spot is in use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation")
		float Cooldown = 3.f;
	UPROPERTY()
		float CooldownTimer = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Activation")
		bool IsInUse = false;

protected:
	// Called whenever a value is changed
	void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the player activates the scare spot
	UFUNCTION(BlueprintCallable, Category = "Activation", DisplayName = "ActivateScareSpot")
		void RecieveActivateScareSpot() { ActivateScareSpot; }
	UFUNCTION(BlueprintImplementableEvent, Category = "Activation")
		void ActivateScareSpot();

};
