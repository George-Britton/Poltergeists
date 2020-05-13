// Fill out your copyright notice in the Description page of Project Settings.

#include "Victim.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Trap.h"
#include "Scares/ScareSpot.h"
#include "Polterguys.h"

// Sets default values
AVictim::AVictim()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ScreamSpeaker = CreateDefaultSubobject<UAudioComponent>(TEXT("Scream Speaker"));
	ScreamSpeaker->SetupAttachment(RootComponent);
	
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
	ScareSpots.Empty();
	
	OnRunAway.Broadcast();
	RunAway();
}

// Called when the victim enters a new room / level
void AVictim::ReceiveEnterNewRoom()
{
	// Destroys the room and resets the fear
	Fear = StartingFear;
	OnEnterNewRoom.Broadcast();
	
	// Registers the new room and increments the round
	Room = UGameplayStatics::GetActorOfClass(this, RoomClass);
	Door = Cast<ADoor>(UGameplayStatics::GetActorOfClass(this, ADoor::StaticClass()));

	// Finds the new scare spots
	TArray<AActor*> TempScareSpots;
	UGameplayStatics::GetAllActorsOfClass(this, AScareSpot::StaticClass(), TempScareSpots);
	for (auto& ScareSpot : TempScareSpots) { ScareSpots.Add(Cast<AScareSpot>(ScareSpot)); }
	
	EnterNewRoom();
	ReceiveRoundStart();
}

// Called every frame
void AVictim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Fear < 100.f && Fear >= 0.f && !RoundOver)
	{
		Fear -= FearDepletionSpeed * DeltaTime;
		if (Fear < 0) Fear = 0;
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
	RoundOver = false;
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

		if (Fear <= 33.f && MildScreams.Num() > 0)
		{
			ScreamSpeaker->SetSound(MildScreams[FMath::RandRange(0, MildScreams.Num() - 1)]);
			ScreamSpeaker->Play();
		}
		else if (Fear <= 66.f && MediumScreams.Num() > 0)
		{
			ScreamSpeaker->SetSound(MediumScreams[FMath::RandRange(0, MediumScreams.Num() - 1)]);
			ScreamSpeaker->Play();
		}
		else if (IntenseScreams.Num() > 0)
		{
			ScreamSpeaker->SetSound(IntenseScreams[FMath::RandRange(0, IntenseScreams.Num() - 1)]);
			ScreamSpeaker->Play();
		}
		else { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "WARNING: NO SCREAMS SET ON " + this->GetName()); }
		
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