// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialAbilityComponent.h"

// Sets default values for this component's properties
USpecialAbilityComponent::USpecialAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Player = Cast<APlayerPoltergeist>(GetOwner());
}

// Called when the player presses the special ability button
void USpecialAbilityComponent::Execute()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Error: No Special Ability on " + GetOwner()->GetName());
}