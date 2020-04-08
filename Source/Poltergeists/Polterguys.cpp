// Fill out your copyright notice in the Description page of Project Settings.


#include "Polterguys.h"
#include "Characters/PlayerPoltergeist.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void APolterguys::BeginPlay()
{
	// Counts how many players there are, and spawns controllers for them all
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerPoltergeist::StaticClass(), Players);
	for (auto& Player : Players) { UGameplayStatics::CreatePlayer(this); }

	AVictim* Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
	Victim->OnRunAway.AddDynamic(this, &APolterguys::OnVictimRunAway);
}

// Called when the victim runs away at full fear
void APolterguys::OnVictimRunAway()
{
	if (RoomClassArray.Num())
	{
		int32 SpawnNum = UKismetMathLibrary::RandomIntegerInRange(0, RoomClassArray.Num() - 1);
		FActorSpawnParameters SpawnParams;
		FVector SpawnLoc = FVector(0, 0, -100000);
		FRotator SpawnRot = FRotator::ZeroRotator;
		AActor* NewRoom = GetWorld()->SpawnActor<AActor>(RoomClassArray[SpawnNum], SpawnLoc, SpawnRot, SpawnParams);
	}
}
