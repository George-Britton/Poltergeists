// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class POLTERGEISTS_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:

	ADoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		USceneComponent* RunToPoint;
	
	// Called when the victim runs away
	UFUNCTION(BlueprintCallable, Category = "Door", DisplayName = "Open")
		void ReceiveOpen();
	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void Open();
};
