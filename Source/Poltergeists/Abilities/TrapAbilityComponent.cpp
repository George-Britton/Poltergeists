// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapAbilityComponent.h"
#include "Characters/PlayerPoltergeist.h"

// Called when the player presses the special ability button
void UTrapAbilityComponent::Execute()
{
	// Spawn a trap
	FActorSpawnParameters SpawnParams;
	FVector SpawnLoc = Player->GetActorLocation();
	SpawnLoc.Z -= Player->GetDefaultHalfHeight();
	ATrap* Trap = GetWorld()->SpawnActor<ATrap>(ATrap::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	// Set trap variables
	Trap->SetMesh(Player->TrapMesh);
	Trap->StartCountdown(TrapTime);
	Trap->TrapStrength = TrapStrength;
	Player->DeclareSpecialDone();
}