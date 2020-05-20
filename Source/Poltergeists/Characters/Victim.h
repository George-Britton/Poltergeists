// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Rooms/Door.h"
#include "Components/AudioComponent.h"
#include "Rooms/Room.h"
#include "Victim.generated.h"

class AScareSpot;
class ATrap;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRunAway);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterNewRoom);

UCLASS()
class POLTERGEISTS_API AVictim : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVictim();
	
	// The current level of fear of the victim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fear")
		float StartingFear = 50.f;
	UPROPERTY(BlueprintReadOnly, Category = "Fear")
		float Fear = 50.f;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		float FearDepletionSpeed = 2.5f;

	// Sounds that are played when the victim is scared
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		USoundBase* MildScreams;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		USoundBase* MediumScreams;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		USoundBase* IntenseScreams;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		USoundBase* RunAwayScream;
	UPROPERTY()
		UAudioComponent* ScreamSpeaker;

	// Sounds that are played when the victim is at different levels of fear
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		USoundBase* MildFearSound;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		USoundBase* IntenseFearSound;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Fear")
		float FearScreamProbabilityPercent = 0.5f;
	
	// How far away the scare spot can be without causing no fear
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fear")
		float MaxScareDistance = 600.f;

	// Array of all the scare spots - places they might run to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scares")
		TArray<AScareSpot*> ScareSpots;

	// Whether the victim is caught in a trap
	UPROPERTY(BlueprintReadOnly, Category = "Trap")
		bool Trapped = false;
		TArray<ATrap*> Traps;

	// Marks the end of the round
	UPROPERTY(BlueprintReadOnly, Category = "Fear")
		bool RoundOver = false;
	UPROPERTY(BlueprintReadOnly, Category = "Rounds")
		int32 Round = 0;
	
	// The door that the victim runs to at the end of the round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		ADoor* Door;

	// The room to despawn on new round start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		ARoom* Room;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
		FVector StartLocation;
		
	// Check for if the player is already running away
	UPROPERTY(BlueprintReadOnly, Category = "Rounds")
		bool IsRunningAway = true;

	// Event dispatcher for when the victim runs for the next room
	UPROPERTY(BlueprintAssignable)
		FOnRunAway OnRunAway;

	// Event dispatcher for when the victim enters the new room
	UPROPERTY(BlueprintAssignable)
		FOnEnterNewRoom OnEnterNewRoom;
	
	// Event dispatcher for when the next round starts
	UPROPERTY(BlueprintAssignable)
		FOnRunAway OnRoundStart;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game is ready for the next room to begin
	UFUNCTION(BlueprintCallable, Category = "Events", DisplayName = "RoundStart")
		void ReceiveRoundStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void RoundStart();

	// Called when the fear meter is full
	UFUNCTION(BlueprintCallable, Category = "Events", DisplayName = "RunAway")
		void ReceiveRunAway();
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void RunAway();

	// Called when the victim enters a new room / level
	UFUNCTION(BlueprintCallable, Category = "Room", DisplayName = "EnterNewRoom")
	void ReceiveEnterNewRoom();
	UFUNCTION(BlueprintImplementableEvent, Category = "Room")
	void EnterNewRoom();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when a scare spot is activated
	UFUNCTION(BlueprintCallable, Category = "Scare", DisplayName = "Scare")
		void ReceiveScare(FVector ScareSource, float ScareStrength);
	UFUNCTION(BlueprintImplementableEvent, Category = "Scare")
		void Scare(FVector ScareSource, float ScareStrength);

	// Called when the victim is caught in a trap
	UFUNCTION(BlueprintCallable, Category = "Trap", DisplayName = "Snare")
		void ReceiveSnare(ATrap* Trap);
	UFUNCTION(BlueprintCallable, Category = "Trap", DisplayName = "UnSnare")
		void ReceiveUnsnare(ATrap* Trap);
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap")
		void Snare(ATrap* Trap);
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap")
		void Unsnare(ATrap* Trap);

	// Called when the victim needs to choose a new scare spot to run to
	UFUNCTION(BlueprintCallable, Category = "Scare")
		AScareSpot* GetRandomScareSpot();
};
