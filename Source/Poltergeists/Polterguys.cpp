// Fill out your copyright notice in the Description page of Project Settings.


#include "Polterguys.h"
#include "Characters/PlayerPoltergeist.h"
#include "Kismet/GameplayStatics.h"

void APolterguys::BeginPlay()
{
	// Counts how many players there are, and spawns controllers for them all
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerPoltergeist::StaticClass(), Players);
	for (auto& Player : Players) { UGameplayStatics::CreatePlayer(this); }

}
