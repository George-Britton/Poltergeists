// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPoltergeist.h"


#include "Abilities/CurseAbilityComponent.h"
#include "Abilities/TimeBombAbilityComponent.h"
#include "Abilities/ToucheAbilityComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Trap.h"
#include "Abilities/TrapAbilityComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerPoltergeist::APlayerPoltergeist()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
	
	// Allow the player to rotate towards their movement vector
	GetCharacterMovement()->bOrientRotationToMovement = 1;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APlayerPoltergeist::BeginPlay()
{
	Super::BeginPlay();

	// Sets the special ability component
	const FTransform SpecialTransform;
	switch(SpecialAbility)
	{
	case EPlayerAbility::TOUCHE: SpecialComponent = Cast<UToucheAbilityComponent>(StaticConstructObject_Internal(UToucheAbilityComponent::StaticClass(), this)); break;
	case EPlayerAbility::CURSE: SpecialComponent = Cast<UCurseAbilityComponent>(StaticConstructObject_Internal(UCurseAbilityComponent::StaticClass(), this)); break;
	case EPlayerAbility::TIMEBOMB: SpecialComponent = Cast<UTimeBombAbilityComponent>(StaticConstructObject_Internal(UTimeBombAbilityComponent::StaticClass(), this)); break;
	case EPlayerAbility::TRAP: SpecialComponent = Cast<UTrapAbilityComponent>(StaticConstructObject_Internal(UTrapAbilityComponent::StaticClass(), this)); break;
	default: break;
	}
	SpecialComponent->RegisterComponent();
	
	// Sets the victim and binds the event listeners for the round timers
	Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
	Victim->OnRunAway.AddDynamic(this, &APlayerPoltergeist::OnRunAway);
	Victim->OnRoundStart.AddDynamic(this,  &APlayerPoltergeist::OnRoundStart);
}

// Called every frame
void APlayerPoltergeist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Reduce the cooldown timer
	if (CooldownTimer > 0) FMath::Clamp<float>(CooldownTimer -= DeltaTime, 0, Cooldown);

	// Reduce ability cooldowns
	if (DashCooldownTimer > 0.f) FMath::Clamp<float>(DashCooldownTimer -= DeltaTime, 0, DashCooldown);
	if (YeetCooldownTimer > 0.f) FMath::Clamp<float>(YeetCooldownTimer -= DeltaTime, 0, YeetCooldown);
	if (SpecialCooldownTimer > 0.f) FMath::Clamp<float>(SpecialCooldownTimer -= DeltaTime, 0, SpecialCooldown);

	// Sends the player into the next room
	if (Chasing)
	{
		FVector DeltaLoc = FVector((RunToLocation - GetActorLocation()).Normalize());
		AddActorWorldOffset(DeltaLoc  * ChaseSpeed);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Victim->GetActorLocation()));
	}
}

// Called whenever the player overlaps with something or stops overlapping
void APlayerPoltergeist::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AScareSpot* TestOverlap = Cast<AScareSpot>(OtherActor);
	if (TestOverlap) OverlappedScareSpot = TestOverlap;

	OverlappingVictim = Cast<AVictim>(OtherActor);
}
void APlayerPoltergeist::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor == OverlappedScareSpot) OverlappedScareSpot = nullptr;
	if (OtherActor == OverlappingVictim) OverlappingVictim = nullptr;
}

// Called to bind functionality to input
void APlayerPoltergeist::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Bind player movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPoltergeist::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPoltergeist::MoveRight);

	// Bind scare activation input
	PlayerInputComponent->BindAction("ActivateScare", IE_Pressed, this, &APlayerPoltergeist::ReceiveActivateScare);

	// Bind ability inputs
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerPoltergeist::Dash);
	PlayerInputComponent->BindAction("Yeet", IE_Pressed, this, &APlayerPoltergeist::Pickup);
	PlayerInputComponent->BindAction("Yeet", IE_Released, this, &APlayerPoltergeist::Yeet);
	PlayerInputComponent->BindAction("Special", IE_Pressed, this, &APlayerPoltergeist::Special);

	// Keep the physics handle in front of the player TODO Stretch goal: fix this and take out blueprint
	//FVector ForwardVecHold = GetActorForwardVector() * ItemHeldDistance;
	//if (IsItemHeld) PhysicsHandle->SetTargetLocation(ForwardVecHold + GetActorLocation());
}

// Input axis for movement
void APlayerPoltergeist::MoveForward(float AxisValue)
{
	APawn::AddMovementInput(FVector(PlayerCamera->GetActorForwardVector().X,
										PlayerCamera->GetActorForwardVector().Y, 
										0),
										AxisValue);
}
void APlayerPoltergeist::MoveRight(float AxisValue)
{
	APawn::AddMovementInput(FVector(PlayerCamera->GetActorRightVector().X,
										PlayerCamera->GetActorRightVector().Y,
										0),
										AxisValue);
}

// Input action for scare activation
void APlayerPoltergeist::ReceiveActivateScare()
{
	if (OverlappedScareSpot && CooldownTimer <= 0)
	{
		if (OverlappedScareSpot->ReceiveActivateScareSpot())
		{
			CooldownTimer = Cooldown;
			ActivateScare();
		}
	}
}

// Input actions for abilities
void APlayerPoltergeist::Dash()
{
	// Player dashes a certain amount in their forward direction
	if (DashCooldownTimer <= 0.f)
	{
		ACharacter::LaunchCharacter(GetActorForwardVector() * DashSpeed, true, false);
		DashCooldownTimer = DashCooldown;
		OnDash.Broadcast();
	}
}
void APlayerPoltergeist::Pickup()
{
	// Box traces for a physics object and picks it up
	if (!IsItemHeld && YeetCooldownTimer <= 0.f)
	{
		FHitResult HitResult;
		FVector TraceToLocation = GetActorForwardVector() * PickupDistance + GetActorLocation();
		TraceToLocation.Z -= 50;
		TArray<TEnumAsByte<EObjectTypeQuery>> YeetTypes;
		YeetTypes.Init(UCollisionProfile::Get()->ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody), 1);
		TArray<AActor*> Ignored;
		if(UKismetSystemLibrary::BoxTraceSingleForObjects(this, GetActorLocation(), TraceToLocation, FVector(40, 40, 160), FRotator::ZeroRotator, YeetTypes, false, Ignored, EDrawDebugTrace::None, HitResult, true))
		{
			if (HitResult.Component->IsSimulatingPhysics())
			{
				PhysicsHandle->GrabComponentAtLocation(HitResult.GetComponent(), "None", HitResult.GetComponent()->GetComponentLocation());
				IsItemHeld = true;
				ComponentHeld = HitResult.GetComponent();
				ObjectBeingHeld = Cast<AYeetable>(HitResult.GetActor());
			}
		}
	}
}
void APlayerPoltergeist::Yeet()
{
	// Throws the held object
	if (IsItemHeld)
	{
		IsItemHeld = false;
		PhysicsHandle->ReleaseComponent();
		FVector YeetDirection = GetActorForwardVector() * YeetStrength;
		YeetDirection.Z += YeetIncline;
		ComponentHeld->AddImpulse(YeetDirection, "None", true);
		ObjectBeingHeld->Yate = true;
		ComponentHeld = nullptr;
		ObjectBeingHeld = nullptr;
		YeetCooldownTimer = YeetCooldown;
	}
}
void APlayerPoltergeist::Special()
{
	if (!SpecialCooldownTimer) SpecialComponent->Execute();
}
void APlayerPoltergeist::DeclareSpecialDone()
{
	SpecialCooldownTimer = SpecialCooldown;
	OnSpecial.Broadcast();
}

// Called when the AI runs away
void APlayerPoltergeist::OnRunAway()
{
	GetCharacterMovement()->StopMovementImmediately();
	DisableInput(Cast<APlayerController>(GetController()));
	Chasing = true;
}
// Called when the AI starts the next round
void APlayerPoltergeist::OnRoundStart()
{
	EnableInput(Cast<APlayerController>(GetController()));
	Chasing = false;
}