// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	MeshComponent->SetCollisionProfileName("OverlapAllDynamic");
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Reduce the time left until the trap gets destroyed
	if (TrapSet) TrapTime -= DeltaTime;
	if (TrapTime <= 0.f)
	{
		if (VictimTrapped) Victim->ReceiveUnsnare(this);
		
		OnDestroy.Broadcast();
		Destroy();
	}
}

// Called whenever the trap overlaps with something or stops overlapping
void ATrap::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!Victim)
	{
		Victim = Cast<AVictim>(OtherActor);
		if (Victim)
		{
			Victim->ReceiveSnare(this);
			VictimTrapped = true;
		}
	}
}
void ATrap::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (Victim == OtherActor)
	{
		Victim->ReceiveUnsnare(this);
		Victim = nullptr;
		VictimTrapped = false;
	}
}

// Sets the static mesh of the trap
void ATrap::SetMesh(UStaticMesh* Mesh)
{
	MeshComponent->SetStaticMesh(Mesh);
}

// Starts the trap destruction countdown
void ATrap::StartCountdown(float Time)
{
	TrapTime = Time;
	TrapSet = true;
}