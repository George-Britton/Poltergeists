// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPoltergeist.h"

#include "Abilities/CurseAbilityComponent.h"
#include "Abilities/TimeBombAbilityComponent.h"
#include "Abilities/ToucheAbilityComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/TrapAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Yeetable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerPoltergeist::APlayerPoltergeist()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
	AbilitySpeaker = CreateDefaultSubobject<UAudioComponent>(TEXT("Ability Speaker"));
	DashParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Dash Particles"));
	AbilitySpeaker->SetupAttachment(RootComponent);
	DashParticleSystem->SetupAttachment(RootComponent);
	AbilitySpeaker->bAutoActivate = false;
	DashParticleSystem->bAutoActivate = false;
	
	// Allow the player to rotate towards their movement vector
	GetCharacterMovement()->bOrientRotationToMovement = 1;
	bUseControllerRotationYaw = false;
}

// Called when a value is changed
void APlayerPoltergeist::OnConstruction(const FTransform& Transform)
{
	if (DashParticleTemplate) DashParticleSystem->SetTemplate(DashParticleTemplate);
}

// Called when the game starts or when spawned
void APlayerPoltergeist::BeginPlay()
{
	Super::BeginPlay();

}

// Called to intialise the controller and player assignment
void APlayerPoltergeist::ReceiveInitialise(int32 ID, int32 SpecialID)
{
	PlayerID = ID;
	
	// Sets the special ability component
	const FTransform SpecialTransform;
	switch(SpecialID)
	{
	case 0: SpecialComponent = NewObject<UToucheAbilityComponent>(this, UToucheAbilityComponent::StaticClass(), NAME_None, RF_Transient); break;
	case 1: SpecialComponent = NewObject<UCurseAbilityComponent>(this, UCurseAbilityComponent::StaticClass(), NAME_None, RF_Transient); break;
	case 2: SpecialComponent = NewObject<UTimeBombAbilityComponent>(this, UTimeBombAbilityComponent::StaticClass(), NAME_None, RF_Transient); break;
	case 3: SpecialComponent = NewObject<UTrapAbilityComponent>(this, UTrapAbilityComponent::StaticClass(), NAME_None, RF_Transient); break;
	default: break;
	}
	SpecialComponent->RegisterComponent();

	
	// Sets the victim and binds the event listeners for the round timers
	Victim = Cast<AVictim>(UGameplayStatics::GetActorOfClass(this, AVictim::StaticClass()));
	Victim->OnRunAway.AddDynamic(this, &APlayerPoltergeist::OnRunAway);
	Victim->OnRoundStart.AddDynamic(this,  &APlayerPoltergeist::OnRoundStart);

	// Creates the player controller
	UGameplayStatics::CreatePlayer(this, ID, true);
	
	Initialise(ID, SpecialID);
}

// Called every frame
void APlayerPoltergeist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Reduce the cooldown timer
	if (CooldownTimer > 0) CooldownTimer -= DeltaTime;

	// Reduce ability cooldowns
	if (DashCooldownTimer > 0.f) DashCooldownTimer -= DeltaTime;
	if (YeetCooldownTimer > 0.f) YeetCooldownTimer -= DeltaTime;
	if (SpecialCooldownTimer > 0.f) SpecialCooldownTimer -= DeltaTime;
}

// Called whenever the player overlaps with something or stops overlapping
void APlayerPoltergeist::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AScareSpot* TestOverlap = Cast<AScareSpot>(OtherActor);
	if (TestOverlap) OverlappedScareSpot = TestOverlap;
	
	AVictim* TestVictim = Cast<AVictim>(OtherActor);
	if (TestVictim) OverlappingVictim = Cast<AVictim>(OtherActor);
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
	if (OverlappedScareSpot && CooldownTimer <= 0.f)
	{
		if (OverlappedScareSpot->ReceiveActivateScareSpot())
		{
			CooldownTimer = Cooldown;
			AbilitySpeaker->Stop();
			AbilitySpeaker->SetSound(ScareSound);
			AbilitySpeaker->Play();
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
		if (DashSound)
		{
			AbilitySpeaker->Stop();
			AbilitySpeaker->SetSound(DashSound);
			AbilitySpeaker->Play();
		}

		if (DashParticleTemplate)
		{
			DashParticleSystem->Activate();
			FTimerHandle DashTimer;
			FTimerDelegate DashDelegate;
			DashDelegate.BindUFunction(this, "OnDashFinish");
			GetWorld()->GetTimerManager().SetTimer(DashTimer, DashDelegate, DashTimeInSeconds, false);
		}
		
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
		if (YeetSound)
		{
			AbilitySpeaker->Stop();
			AbilitySpeaker->SetSound(YeetSound);
			AbilitySpeaker->Play();
		}
	}
}
void APlayerPoltergeist::Special()
{
	if (SpecialCooldownTimer <= 0.f) SpecialComponent->Execute();
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
	ChaseState = EChaseState::FADING_OUT;
}
// Called when the AI starts the next round
void APlayerPoltergeist::OnRoundStart()
{
	EnableInput(Cast<APlayerController>(GetController()));
	ChaseState = EChaseState::PLAYING;
}