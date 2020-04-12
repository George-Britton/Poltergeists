// Fill out your copyright notice in the Description page of Project Settings.

#include "Victim.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Polterguys.h"

// Sets default values
AVictim::AVictim()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialise the scare spots array
	ScareSpots.Init(nullptr, 1);
}

// Called when the game starts or when spawned
void AVictim::BeginPlay()
{
	Super::BeginPlay();

	ReceiveEnterNewRoom();
	
	ReceiveRoundStart();
}

// Called when the fear meter is full
void AVictim::ReceiveRunAway()
{
	OnRunAway.Broadcast();
	RunAway();
}

// Called when the victim enters a new room / level
void AVictim::ReceiveEnterNewRoom()
{
	if (Round > 0)
	{
		// Gets all the scare spots, and fills the array
		ScareSpots.Empty();
		if (Room) Room->Destroy();

		TSubclassOf<AActor> RoomClass;
		for (int32 RoomClassFinder = 0; RoomClassFinder < RoomArray.Num(); ++RoomClassFinder)
		{
			if (UGameplayStatics::GetActorOfClass(this, RoomArray[RoomClassFinder]) != nullptr)
			{
				RoomClass = RoomArray[RoomClassFinder];
				break;
			}
		}
		Room = UGameplayStatics::GetActorOfClass(this, RoomClass);
		
		TArray<AActor*> ScareSpotFinderArray;
		UGameplayStatics::GetAllActorsOfClass(this, AScareSpot::StaticClass(), ScareSpotFinderArray);
		for (auto& ScareSpot : ScareSpotFinderArray) ScareSpots.Add(Cast<AScareSpot>(ScareSpot));
		Fear = StartingFear;
	}
	++Round;
	EnterNewRoom();
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Fear < 100.f) FMath::Clamp<float>(Fear -= FearDepletionSpeed * DeltaTime, 0, 100);
	else if (!RoundOver)
	{
		RoundOver = true;
		ReceiveRunAway();
	}
}

// Called when the game is ready for the next room to begin
void AVictim::ReceiveRoundStart()
{
	Door = Cast<ADoor>(UGameplayStatics::GetActorOfClass(this, ADoor::StaticClass()));
	OnRoundStart.Broadcast();
	RoundStart();
}

// Called when a scare spot is activated
void AVictim::ReceiveScare(FVector ScareSource, float ScareStrength)
{
	// Check the scare is within range
	FVector VectorDistanceToScare = ScareSource - GetActorLocation();
	float DistanceToScare = VectorDistanceToScare.Size();

	if (DistanceToScare <= MaxScareDistance)
	{
		float FearToAdd = DistanceToScare / MaxScareDistance;
		FearToAdd = 1 - FearToAdd;
		FearToAdd *= ScareStrength;
		Fear += FearToAdd;
		Scare(ScareSource, ScareStrength);
	}
}

// Called when the victim is caught in a trap
void AVictim::ReceiveSnare(ATrap* Trap)
{
	Trapped = true;
	Traps.Add(Trap);
	Snare(Trap);
}
void AVictim::ReceiveUnsnare(ATrap* Trap)
{
	Traps.Remove(Trap);
	if (Traps.Num() == 0) Trapped = false;
	Unsnare(Trap);
}

// Called when the victim needs to choose a new scare spot to run to
AScareSpot* AVictim::GetRandomScareSpot()
{
	if (ScareSpots.Num() != 0)
		return ScareSpots[FMath::RandRange(0, ScareSpots.Num() - 1)];

	return nullptr;
}