// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SpecialAbilityComponent.h"
#include "TimeBombAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERGEISTS_API UTimeBombAbilityComponent : public USpecialAbilityComponent
{
	GENERATED_BODY()

public:

	// The delay before setting the timebomb off
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float TimeBombDelay = 3.f;

	// Called when the player presses the special ability button
	void Execute() override;
};
