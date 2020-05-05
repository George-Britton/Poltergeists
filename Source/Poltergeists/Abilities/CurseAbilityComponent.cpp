// Fill out your copyright notice in the Description page of Project Settings.


#include "CurseAbilityComponent.h"
#include "Characters/PlayerPoltergeist.h"

// Called when the player presses the special ability button
void UCurseAbilityComponent::Execute()
{
	if (Player->OverlappedScareSpot)
	{
		if (Player->OverlappedScareSpot->Curse(CurseMultiplier, CurseTime)) { Player->DeclareSpecialDone(); }
	}
}
