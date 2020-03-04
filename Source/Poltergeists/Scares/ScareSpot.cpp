// Fill out your copyright notice in the Description page of Project Settings.


#include "ScareSpot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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
	RootComponent->SetMobility(EComponentMobility::Movable);
	ScareSpotMeshComponent->SetMobility(EComponentMobility::Movable);
}

// Called whenever a value is changed
void AScareSpot::OnConstruction(const FTransform& Transform)
{
	if (ScareSpotMesh) ScareSpotMeshComponent->SetStaticMesh(ScareSpotMesh);
		ActivationSphere->SetSphereRadius(ActivationDistance);

	ActivationSphere->SetRelativeLocation(ActivationSphereRelativeLocation);
	
	RechargeTimer = RechargeTime;
}

// Called every frame
void AScareSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Reduce active timer until scare stops
	if (ActiveTimer > 0.f) FMath::Clamp<float>(ActiveTimer -= DeltaTime, 0, ActiveTime);

	// Set usable again
	if (ActiveTimer <= 0.f && ScareState == EScareState::ACTIVE) BeginRecharge();

	// Reduce cooldown timer until available again
	if (ScareState == EScareState::RECHARGING) FMath::Clamp<float>(RechargeTimer += DeltaTime, 0, RechargeTime);
	
	// Start the cooldown period
	if (RechargeTimer >= RechargeTime && ScareState == EScareState::RECHARGING) ResetScareSpot();
}

// Called when the player activates the scare spot
bool AScareSpot::ReceiveActivateScareSpot()
{
	// Scares the victim if possible
	if (ScareState == EScareState::READY || ScareState == EScareState::RECHARGING)
	{
		// Sets the victim if it doesn't already exist
		if (!Victim) Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
		
		ScareState = EScareState::ACTIVE;
		ActiveTimer = ActiveTime;
		ActivateScareSpot();
		Victim->ReceiveScare(GetActorLocation(), ScareStrength * (RechargeTimer / RechargeTime));
		
		return true;
	}
	return false;
}

// Called when the scare spot finishes scaring and starts the cooldown period
void AScareSpot::BeginRecharge()
{
	OnScareFinish();
	
	if (Retriggerable)
	{
		ScareState = EScareState::RECHARGING;
		RechargeTimer = 0;
	} else ScareState = EScareState::OFF;
}

// Called when the scare spot is made active again
void AScareSpot::ResetScareSpot()
{
	ScareState = EScareState::READY;
	RechargeTimer = RechargeTime;
	OnScareReset();
}
