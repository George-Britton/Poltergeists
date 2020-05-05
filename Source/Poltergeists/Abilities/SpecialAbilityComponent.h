// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpecialAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLTERGEISTS_API USpecialAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpecialAbilityComponent();

	// The player the component is attached to
	UPROPERTY()
		class APlayerPoltergeist* Player;
	
	// Called when the player presses the special ability button
	virtual void Execute();
};
