// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScareSpot.h"
#include "ScareSpotRandomiser.generated.h"

UCLASS()
class POLTERGEISTS_API AScareSpotRandomiser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScareSpotRandomiser();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scares")
		TArray<TSubclassOf<AScareSpot>> ScareSpots;

	UPROPERTY(BlueprintReadOnly, Category = "Scare")
		AScareSpot* SpawnedScareSpot;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the room is destroyed
	UFUNCTION(BlueprintCallable, Category = "Events")
		void OnDeletion();
};
