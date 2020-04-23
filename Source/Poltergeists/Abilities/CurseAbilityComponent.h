// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SpecialAbilityComponent.h"
#include "CurseAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class POLTERGEISTS_API UCurseAbilityComponent : public USpecialAbilityComponent
{
	GENERATED_BODY()

public:

	// The power and time of the curse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float CurseMultiplier = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float CurseTime = 30.f;

	// Called when the player presses the special ability button
	void Execute() override;
};
