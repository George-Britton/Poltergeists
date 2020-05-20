// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RunToPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Run to point"));
	RunToPoint->SetupAttachment(RootComponent);
}

// Called when the victim runs away
void ADoor::ReceiveOpen()
{
	Open();
}
