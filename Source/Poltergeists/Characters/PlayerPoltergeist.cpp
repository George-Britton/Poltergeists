// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPoltergeist.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Trap.h"
#include "Components/StaticMeshComponent.h"
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
	if (!IsItemHeld)
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
	// Checks which ability the player has and redirects to that function
	if (SpecialCooldownTimer <= 0)
		switch (SpecialAbility)
		{
		case EPlayerAbility::CURSE: Curse(); break;
		case EPlayerAbility::TOUCHE: Touche(); break;
		case EPlayerAbility::TIMEBOMB: TimeBomb(); break;
		case EPlayerAbility::TRAP: Trap(); break;
		default: break;
		}
}
void APlayerPoltergeist::Curse()
{
	if (OverlappedScareSpot)
	{
		if (OverlappedScareSpot->Curse(CurseMultiplier, CurseTime)) { DeclareSpecialDone(); }
	}
}
void APlayerPoltergeist::Touche()
{
	if (OverlappingVictim)
	{
		OverlappingVictim->ReceiveScare(GetActorLocation(), ToucheStrength);
		DeclareSpecialDone();
	}
}
void APlayerPoltergeist::TimeBomb()
{
	if (OverlappedScareSpot)
	{
		OverlappedScareSpot->TimeBomb(TimeBombDelay);
		DeclareSpecialDone();
	}
}
void APlayerPoltergeist::Trap()
{
	// Spawn a trap
	FActorSpawnParameters SpawnParams;
	FVector SpawnLoc = GetActorLocation();
	SpawnLoc.Z -= GetDefaultHalfHeight();
	ATrap* Trap = GetWorld()->SpawnActor<ATrap>(ATrap::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	// Set trap variables
	Trap->SetMesh(TrapMesh);
	Trap->StartCountdown(TrapTime);
	Trap->TrapStrength = TrapStrength;
	DeclareSpecialDone();
}
void APlayerPoltergeist::DeclareSpecialDone()
{
	SpecialCooldownTimer = SpecialCooldown;
	OnSpecial.Broadcast();
}
