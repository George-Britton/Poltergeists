// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Polterguys.generated.h"

/**
 * 
 */
UCLASS()
class POLTERGEISTS_API APolterguys : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Room classes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
		TArray<TSubclassOf<AActor>> RoomClassArray;
	
	// Called when the victim runs away at full fear
	UFUNCTION()
	void OnVictimRunAway();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
