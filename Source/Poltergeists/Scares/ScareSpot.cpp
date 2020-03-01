// Fill out your copyright notice in the Description page of Project Settings.


#include "ScareSpot.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AScareSpot::AScareSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ScareSpotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ActivationSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Activation Sphere"));

	// Connect all the components to the root
	ScareSpotMeshComponent->SetupAttachment(RootComponent);
	ActivationSphere->SetupAttachment(RootComponent);
}

// Called whenever a value is changed
void AScareSpot::OnConstruction(const FTransform& Transform)
{
	if (ScareSpotMesh) ScareSpotMeshComponent->SetStaticMesh(ScareSpotMesh);
	ActivationSphere->SetSphereRadius(ActivationDistance);

	ActivationSphere->SetRelativeLocation(ActivationSphereRelativeLocation);
}

// Called every frame
void AScareSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Reduce active timer until scare stops
	if (ActiveTimer > 0.f) ActiveTimer -= DeltaTime;

	// Set usable again
	if (ActiveTimer <= 0.f && ScareState == EScareState::ACTIVE) BeginCooldown();

	// Reduce cooldown timer until available again
	if (CooldownTimer > 0) CooldownTimer -= DeltaTime;
	
	// Start the cooldown period
	if (CooldownTimer <= 0.f && ScareState == EScareState::COOLDOWN) ResetScareSpot();
}

// Called when the player activates the scare spot
void AScareSpot::ReceiveActivateScareSpot()
{
	 if (ScareState == EScareState::READY)
	 {
	 	ScareState = EScareState::ACTIVE;
	 	ActiveTimer = ActiveTime;
		ActivateScareSpot();
	 }
}

// Called when the scare spot finishes scaring and starts the cooldown period
void AScareSpot::BeginCooldown()
{
	OnScareFinish();
	
	if (Retriggerable)
	{
		ScareState = EScareState::COOLDOWN;
		CooldownTimer = CooldownTime;
	} else ScareState = EScareState::OFF;
}

// Called when the scare spot is made active again
void AScareSpot::ResetScareSpot()
{
	ScareState = EScareState::READY;
	OnScareReset();
}
