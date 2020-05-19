// Copyright by George Britton and Valeria Lakhtikova 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

UCLASS()
class POLTERGEISTS_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Deletion")
		void MarkForDeletion();
	UFUNCTION(BlueprintImplementableEvent, Category = "Deletion")
		void Delete();
};
