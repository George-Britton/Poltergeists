// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Victim.h"
#include "Yeetable.generated.h"

UCLASS()
class POLTERGEISTS_API AYeetable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AYeetable();

	// The mesh of the yeetable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MeshComponent;

	// Yate - a yeet, a past tense yeet
	UPROPERTY(BlueprintReadOnly, Category = "Yeet")
		bool Yate = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yeet")
		float ScareStrength = 15.f;

	// the victim of the scare
	UPROPERTY(BlueprintReadOnly, Category = "Victim")
		AVictim* Victim;

protected:
	// Called when a value is changed
	void OnConstruction(const FTransform& Transform) override;

public:
	// Called whenever the actor hits something
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	void SetMesh(UStaticMesh* StaticMesh);
};
