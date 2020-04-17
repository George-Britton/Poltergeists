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
	// Destroys the room and resets the fear
	if (Round > 0)
	{
		ScareSpots.Empty();
		if (Room) Room->Destroy();
		if (Door) Door->Destroy();
		
		Fear = StartingFear;
	}

	// Registers the new room and increments the round
	Room = UGameplayStatics::GetActorOfClass(this, RoomClass);
	Door = Cast<ADoor>(UGameplayStatics::GetActorOfClass(this, ADoor::StaticClass()));

	OnEnterNewRoom.Broadcast();
	EnterNewRoom();
	ReceiveRoundStart();
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Fear < 100.f && Fear > 0.f)
	{
		Fear -= FearDepletionSpeed * DeltaTime;
		FMath::Clamp<float>(Fear, 0, 100);
	}
	else if (!RoundOver && Fear >= 100)
	{
		RoundOver = true;
		ReceiveRunAway();
	}
}

// Called when the game is ready for the next room to begin
void AVictim::ReceiveRoundStart()
{
	++Round;
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