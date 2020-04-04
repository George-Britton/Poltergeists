// Fill out your copyright notice in the Description page of Project Settings.

#include "Victim.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

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
	
	RoundStart();
}

// Called when the fear meter is full
void AVictim::ReceiveRunAway()
{
	RunAway();
}

// Called when the victim enters a new room / level
void AVictim::ReceiveEnterNewRoom()
{
	// Gets all the scare spots, and fills the array
	ScareSpots.Empty();
	if (Room) Room->Destroy();
	Room = UGameplayStatics::GetActorOfClass(this, RoomClass);
	TArray<AActor*> ScareSpotFinderArray;
	UGameplayStatics::GetAllActorsOfClass(this, AScareSpot::StaticClass(), ScareSpotFinderArray);
	for (auto& ScareSpot : ScareSpotFinderArray) ScareSpots.Add(Cast<AScareSpot>(ScareSpot));
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
