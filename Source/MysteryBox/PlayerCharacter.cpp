// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MysteryBoxActor.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Movement Setup
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);

	// Game Variables Initialization
	FragmentCount = 0;
	bIsStunned = false;
	BaseSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//// Add Input Mapping Context (Only applies if possessed by a local player)
	//if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//	}
	//}
}


void APlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	// This function guarantees the Controller is fully possessed and valid.
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Clear existing mappings just in case it runs twice, then add it
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsStunned) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Move based on absolute world directions for a fixed top-down camera
		const FVector ForwardDirection = FVector::ForwardVector;
		const FVector RightDirection = FVector::RightVector;

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}


void APlayerCharacter::Interact()
{
	if (bIsStunned) return;

	// Sphere trace slightly in front of the player to find a box
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() * 150.0f);

	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Don't hit ourselves

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams);

	if (bHit)
	{
		// 1. Attempt to cast the hit actor to a Mystery Box
		AMysteryBoxActor* HitBox = Cast<AMysteryBoxActor>(HitResult.GetActor());

		if (HitBox)
		{
			// 2. We hit a box. Call its OpenBox function.
			// This triggers the box's cooldown and returns the color it currently is.
			EBoxColor OpenedColor = HitBox->OpenBox();

			// 3. Print the result to the screen (debug)
			if (GEngine)
			{
				// Convert the Enum to a readable string
				FString ColorString = UEnum::GetValueAsString(OpenedColor);

				// Optional: Strip out the "EBoxColor::" prefix for a cleaner debug message
				ColorString.Split(TEXT("::"), nullptr, &ColorString);

				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("Player opened a %s box!"), *ColorString));
			}

			// rng logic will be here
		}
	}
}


// Status Effects
void APlayerCharacter::ApplySpeedModifier(float Multiplier, float Duration)
{
	// Placeholder for applying speed buff/debuff
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("ApplySpeedModifier Triggered"));
}


void APlayerCharacter::ResetSpeed()
{
	// Placeholder for resetting speed
}


void APlayerCharacter::ApplyStun(float Duration)
{
	// Placeholder for applying stun
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ApplyStun Triggered"));
}


void APlayerCharacter::ResetStun()
{
	// Placeholder for removing stun
}

