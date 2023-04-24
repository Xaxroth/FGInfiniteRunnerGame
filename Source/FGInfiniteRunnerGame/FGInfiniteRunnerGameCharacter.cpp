// Copyright Epic Games, Inc. All Rights Reserved.

#include "FGInfiniteRunnerGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AEndlessRunnerProjectCharacter

AFGInfiniteRunnerGameCharacter::AFGInfiniteRunnerGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->AirControl = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFGInfiniteRunnerGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AFGInfiniteRunnerGameCharacter::OnOverlapBegin);

	GetCharacterMovement()->BrakingDecelerationWalking = 10000.f;
	// Rotation speed for character (0 = no turning at all, character always faces forward)
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	GetCharacterMovement()->AirControl = 10000.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	CameraBoom->TargetArmLength = 600.0f;

	SetActorLocation(FVector(0.f, 0.f, 0.f));	

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SpawnInitialTile();
}

void AFGInfiniteRunnerGameCharacter::SpawnInitialTile() 
{
	if (InitialTile != NULL)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			FRotator rotator;

			FVector spawnLocation = GetActorLocation() + FVector(0, 0, -500.f);

			world->SpawnActor<AInfiniteFloorTile>(InitialTile, spawnLocation, rotator, spawnParams);
		}
	}
}

void AFGInfiniteRunnerGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->MaxWalkSpeed < 5000)
	{
		// Increase speed OT
		GetCharacterMovement()->MaxWalkSpeed += 1;
	}
	else
	{
		// Whatever max value wanted
		GetCharacterMovement()->MaxWalkSpeed = 5000;
	}

	GetCharacterMovement()->MinAnalogWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	FRotator ControlRotation = GetControlRotation();

	ControlRotation.Roll = 0.f;
	ControlRotation.Yaw = 0.f;

	AddMovementInput(ControlRotation.Vector());

	AFGInfiniteRunnerGameGameMode* GameMode = Cast<AFGInfiniteRunnerGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		if (GameMode->PlayerHealth <= 0)
		{
			PlayerDead = true;
		}

		if (!PlayerDead)
		{
			GameMode->IncreaseScore(1);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("PLAYER SCORE: %d"), GameMode->PlayerScore));
		}
		else
		{
			Destroy();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFGInfiniteRunnerGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFGInfiniteRunnerGameCharacter::Move);

		//Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFGInfiniteRunnerGameCharacter::Look);

	}

}

void AFGInfiniteRunnerGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, 0, 0);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Only accepts movement input for right-left (x axis)
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFGInfiniteRunnerGameCharacter::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check if the other actor is an obstacle
	if (OtherActor->ActorHasTag("Obstacle"))
	{
		// Destroy the actor
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Obstacle! Life lost")));
		//GameManager->ObstacleHit();
		//Destroy();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Not an obstacle")));
		//Destroy();
	}
}




