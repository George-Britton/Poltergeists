// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeBombAbilityComponent.h"

// Called when the player presses the special ability button
void UTimeBombAbilityComponent::Execute()
{
	if (Player->OverlappedScareSpot)
	{
		Player->OverlappedScareSpot->TimeBomb(TimeBombDelay);
		Player->DeclareSpecialDone();
	}
}