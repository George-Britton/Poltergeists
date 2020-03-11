// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Victim.h"
#include "Trap.generated.h"

class AVictim;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroy);

UCLASS()
class POLTERGEISTS_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

	// The mesh for the trap
	UPROPERTY()
		UStaticMeshComponent* MeshComponent;

	// The timer and activation of the trap
	UPROPERTY(BlueprintReadOnly, Category = "Trap")
		float TrapTime = 0.f;
		bool TrapSet = false;
		bool VictimTrapped = false;

	// The strength of the trap
	UPROPERTY(BlueprintReadOnly, Category = "Trap")
		float TrapStrength = 0.f;

	// The victim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victim")
		AVictim* Victim;

	// Event Dispatcher for the destruction of the trap
	UPROPERTY(BlueprintAssignable, Category = "Trap")
		FOnDestroy OnDestroy;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called whenever the trap overlaps with something or stops overlapping
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	// Sets the static mesh of the trap
	void SetMesh(UStaticMesh* Mesh);

	// Starts the trap destruction countdown
	void StartCountdown(float Time);

};
