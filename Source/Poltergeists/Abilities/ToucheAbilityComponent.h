// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SpecialAbilityComponent.h"
#include "Scares/ScareSpot.h"
#include "ToucheAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERGEISTS_API UToucheAbilityComponent : public USpecialAbilityComponent
{
	GENERATED_BODY()

public:
	
	// The scare strength of the touche
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float ToucheStrength = 80.f;

	// Called when the player presses the special ability button
	void Execute() override;
};
