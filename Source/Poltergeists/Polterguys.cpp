// Fill out your copyright notice in the Description page of Project Settings.


#include "Polterguys.h"
#include "Characters/PlayerPoltergeist.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void APolterguys::BeginPlay()
{
	if (GetWorld()->GetName() != "MenuMap" && GetWorld()->GetName() != "EndMap")
	{
		Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
		Victim->OnRunAway.AddDynamic(this, &APolterguys::OnVictimRunAway);
	}
}

// Called when the victim runs away at full fear
void APolterguys::OnVictimRunAway()
{
	if (RoomClassArray.Num())
	{
		int32 NewRoomNumber;
		do{
			NewRoomNumber = UKismetMathLibrary::RandomIntegerInRange(0, RoomClassArray.Num() - 1);
		} while (NewRoomNumber == LastRoomNumber);
		LastRoomNumber = NewRoomNumber;
		FActorSpawnParameters SpawnParams;
		FVector SpawnLoc = Victim->Door->GetActorLocation();
		FRotator SpawnRot = FRotator::ZeroRotator;
		AActor* NewRoom = GetWorld()->SpawnActor<AActor>(RoomClassArray[NewRoomNumber], SpawnLoc, SpawnRot, SpawnParams);
	}
}
