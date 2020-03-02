// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPoltergeist.h"

// Sets default values
APlayerPoltergeist::APlayerPoltergeist()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPoltergeist::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPoltergeist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called whenever the player overlaps with something or stops overlapping
void APlayerPoltergeist::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AScareSpot* TestOverlap = Cast<AScareSpot>(OtherActor);
	if (TestOverlap) OverlappedScareSpot = TestOverlap;
}
void APlayerPoltergeist::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor == OverlappedScareSpot) OverlappedScareSpot = nullptr;
}

// Called to bind functionality to input
void APlayerPoltergeist::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind player movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPoltergeist::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPoltergeist::MoveRight);

	// Bind scare activation input
	PlayerInputComponent->BindAction("ActivateScare", IE_Pressed, this, &APlayerPoltergeist::ActivateScareSpot);
}

// Input axis for movement
void APlayerPoltergeist::MoveForward(float AxisValue)
{
	APawn::AddMovementInput(FVector(PlayerCamera->GetActorForwardVector().X,
										PlayerCamera->GetActorForwardVector().Y, 
										0),
										AxisValue);
}
void APlayerPoltergeist::MoveRight(float AxisValue)
{
	APawn::AddMovementInput(FVector(PlayerCamera->GetActorRightVector().X,
										PlayerCamera->GetActorRightVector().Y,
										0),
										AxisValue);
}

// Input action for scare activation
void APlayerPoltergeist::ActivateScareSpot()
{
	if (OverlappedScareSpot) OverlappedScareSpot->ReceiveActivateScareSpot();
}
