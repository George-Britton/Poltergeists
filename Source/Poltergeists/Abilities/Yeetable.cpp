// Fill out your copyright notice in the Description page of Project Settings.


#include "Yeetable.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerPoltergeist.h"

// Sets default values
AYeetable::AYeetable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComponent->SetCollisionProfileName("PhysicsBody");
	MeshComponent->SetNotifyRigidBodyCollision(true);

}

// Called when a value is changed
void AYeetable::OnConstruction(const FTransform& Transform)
{
	if (Mesh) MeshComponent->SetStaticMesh(Mesh);
}

// Called whenever the actor hits something
void AYeetable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Yate && OtherActor->GetClass() != APlayerPoltergeist::StaticClass())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, this->GetName() + " hit " + OtherActor->GetName());
		if (!Victim) Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
		
		Victim->ReceiveScare(GetActorLocation(), ScareStrength);
		Yate = false;
	}
}

void AYeetable::SetMesh(UStaticMesh* StaticMesh)
{
	MeshComponent->SetStaticMesh(StaticMesh);
}
