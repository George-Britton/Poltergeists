// Fill out your copyright notice in the Description page of Project Settings.


#include "ScareSpotRandomiser.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AScareSpotRandomiser::AScareSpotRandomiser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void AScareSpotRandomiser::BeginPlay()
{
	Super::BeginPlay();

	if (ScareSpots.Num() != 0)
	{
		int32 SpawnNum = UKismetMathLibrary::RandomIntegerInRange(0, ScareSpots.Num() - 1);
		FActorSpawnParameters SpawnParams;
		FVector SpawnLoc = GetActorLocation();
		FRotator SpawnRot = GetActorRotation();
		SpawnedScareSpot = GetWorld()->SpawnActor<AScareSpot>(ScareSpots[SpawnNum], SpawnLoc, SpawnRot, SpawnParams);
	}
}

// Called when the room is destroyed
void AScareSpotRandomiser::OnDeletion()
{
	SpawnedScareSpot->Destroy();
	Destroy();
}
