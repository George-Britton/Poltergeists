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
}

// Called every frame
void AScareSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Reduce cooldown timer until available again
	if (CooldownTimer > 0.f) CooldownTimer -= DeltaTime;

	// Set usable again
	if (CooldownTimer <= 0.f && IsInUse) IsInUse = false;
}
