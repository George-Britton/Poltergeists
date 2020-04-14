// Fill out your copyright notice in the Description page of Project Settings.


#include "ToucheAbilityComponent.h"

// Called when the player presses the special ability button
void UToucheAbilityComponent::Execute()
{
	if (Player->OverlappingVictim)
	{
		Player->OverlappingVictim->ReceiveScare(Player->GetActorLocation(), ToucheStrength);
		Player->DeclareSpecialDone();
	}
}