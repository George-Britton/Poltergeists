// Fill out your copyright notice in the Description page of Project Settings.


#include "ToucheAbilityComponent.h"
#include "Characters/PlayerPoltergeist.h"

// Called when the player presses the special ability button
void UToucheAbilityComponent::Execute()
{
	if (Player->OverlappingVictim)
	{
		Player->Victim->ReceiveScare(Player->GetActorLocation(), ToucheStrength);
		Player->DeclareSpecialDone();
		if (!ActivationSpeaker->Sound) ActivationSpeaker->SetSound(Player->ToucheSound);
		ActivationSpeaker->Play();
	}
}