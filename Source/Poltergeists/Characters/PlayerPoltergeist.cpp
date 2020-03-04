// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPoltergeist.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerPoltergeist::APlayerPoltergeist()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	if (DashCooldownTimer > 0) FMath::Clamp<float>(DashCooldownTimer -= DeltaTime, 0, DashCooldown);
}

// Called whenever the player overlaps with something or stops overlapping
void APlayerPoltergeist::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AScareSpot* TestOverlap = Cast<AScareSpot>(OtherActor);
	if (TestOverlap) OverlappedScareSpot = TestOverlap;
}
void APlayerPoltergeist::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor == OverlappedScareSpot) OverlappedScareSpot = nullptr;
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
	if (DashCooldownTimer <= 0.f)
	{
		ACharacter::LaunchCharacter(GetActorForwardVector() * DashSpeed, true, false);
		DashCooldownTimer = DashCooldown;
		OnDash.Broadcast();
	}
}
void APlayerPoltergeist::Pickup()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "1");
	if (!IsItemHeld)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "2");
		FHitResult HitResult;
		FVector TraceToLocation = GetActorForwardVector() * PickupDistance + GetActorLocation();
		TraceToLocation.Y -= 20;
		TArray<TEnumAsByte<EObjectTypeQuery>> YeetTypes;
		TArray<AActor*> Ignored;
		if(UKismetSystemLibrary::BoxTraceSingleForObjects(this, GetActorLocation(), TraceToLocation, FVector::ZeroVector, FRotator::ZeroRotator, YeetTypes, false, Ignored, EDrawDebugTrace::ForDuration, HitResult, true, FColor::Red, FColor::Green, 5))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "3");
			if (HitResult.Component->IsSimulatingPhysics())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "4");
				FTransform RelTransform;
				PhysicsHandle = Cast<UPhysicsHandleComponent>(AddComponent("UPhysicsHandleComponent", true, RelTransform, NULL));
				AddOwnedComponent(PhysicsHandle);
				PhysicsHandle->GrabComponentAtLocation(HitResult.GetComponent(), "None", HitResult.Location);
				IsItemHeld = true;
				ObjectBeingHeld = HitResult.GetComponent();
			}
		}
	}
}
void APlayerPoltergeist::Yeet()
{
	if (IsItemHeld)
	{
		PhysicsHandle->ReleaseComponent();
		PhysicsHandle->DestroyComponent();
		PhysicsHandle = nullptr;
		IsItemHeld = false;
		ObjectBeingHeld->AddImpulse(GetActorForwardVector() * YeetStrength, "None", true);
		YeetCooldownTimer = YeetCooldown;
	}
}