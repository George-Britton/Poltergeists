// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Scares/ScareSpot.h"
#include "Victim.generated.h"

class AScareSpot;

UCLASS()
class POLTERGEISTS_API AVictim : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVictim();

	// The current level of fear of the victim
	UPROPERTY(BlueprintReadOnly, Category = "Fear")
		float Fear = 50.f;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		float FearDepletionSpeed = 5.f;

	// How far away the scare spot can be without causing no fear
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fear")
		float MaxScareDistance = 300.f;

	// Array of all the scare spots - places they might run to
	UPROPERTY(BlueprintReadOnly, Category = "Scares")
		TArray<AScareSpot*> ScareSpots;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the victim enters a new room / level
	void EnterNewRoom();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when a scare spot is activated
	UFUNCTION(BlueprintCallable, Category = "Scare", DisplayName = "Scare")
		void ReceiveScare(FVector ScareSource, float ScareStrength);
	UFUNCTION(BlueprintImplementableEvent, Category = "Scare")
		void Scare(FVector ScareSource, float ScareStrength);
};
