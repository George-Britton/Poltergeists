// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SpecialAbilityComponent.h"
#include "TrapAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERGEISTS_API UTrapAbilityComponent : public USpecialAbilityComponent
{
	GENERATED_BODY()

public:

	// The strength, lifetime, and appearance of the trap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float TrapStrength = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float TrapTime = 4.f;

	// Called when the player presses the special ability button
	void Execute() override;
};
