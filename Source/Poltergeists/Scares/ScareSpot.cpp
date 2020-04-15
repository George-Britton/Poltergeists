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

// Called when the game is ready for the next room to begin
void AScareSpot::BeginPlay()
{
	Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
	Victim->OnRoundStart.AddDynamic(this, & AScareSpot::OnRoundStart);
}

// Called every frame
void AScareSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Reduce active timer until scare stops
	if (ActiveTimer > 0.f) FMath::Clamp<float>(ActiveTimer -= DeltaTime, 0, ActiveTime);
	// Set usable again
	if (ActiveTimer <= 0.f && ScareState == EScareState::ACTIVE) BeginRecharge();

	// Reduce cooldown timer until full power
	if (ScareState == EScareState::RECHARGING) RechargeTimer += DeltaTime;
	// Start the cooldown period
	if (RechargeTimer >= RechargeTime && ScareState == EScareState::RECHARGING) ResetScareSpot();

	// Count down the curse timer
	if (CurseTimer > 0.f && IsCursed) CurseTimer -= DeltaTime;
	else if (CurseTimer <= 0.f && IsCursed) Curse(0, 0);

	// Count down the fuses for any time bombs on the scare spot
	if (TimeBombFuses.Num() != 0) {
		TArray<int32> FusesToRemove;
		for (int32 Fuse = 0; Fuse < TimeBombFuses.Num(); ++Fuse)
		{
			TimeBombFuses[Fuse] -= DeltaTime;
			if (TimeBombFuses[Fuse] <= 0.f)
			{
				ReceiveActivateScareSpot();
				FusesToRemove.Add(Fuse);
			}
		}
		for (auto& Fizzle : FusesToRemove) { TimeBombFuses.RemoveAt(Fizzle); }
		FusesToRemove.Empty();
	}
	
}

// Called when the player activates the scare spot
bool AScareSpot::ReceiveActivateScareSpot()
{
	// Scares the victim if possible
	if (ScareState == EScareState::READY || ScareState == EScareState::RECHARGING)
	{		
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

// Called when the curse boi curses the scare spot
bool AScareSpot::Curse(float Multiplier, float Time)
{
	if (!IsCursed)
	{
		IsCursed = true;
		CurseTimer = Time;
		CurseResetStrength = ScareStrength;
		ScareStrength *= Multiplier;
		return true;
	}

	if (CurseTimer <= 0.f)
	{
		IsCursed = false;
		ScareStrength = CurseResetStrength;
		return false;
	}

	return false;
}

// Called when the time bomb sets and explodes
void AScareSpot::TimeBomb(float Time)
{
	TimeBombFuses.Add(Time);
}

// Called when the victim starts the round
void AScareSpot::OnRoundStart()
{
	Victim->ScareSpots.Add(this);
}
