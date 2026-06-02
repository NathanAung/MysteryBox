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
		// Attempt to cast the hit actor to a Mystery Box
		AMysteryBoxActor* HitBox = Cast<AMysteryBoxActor>(HitResult.GetActor());

		if (HitBox)
		{
			// We hit a box. Call its OpenBox function.
			// This triggers the box's cooldown and returns the color it currently is.
			EBoxColor OpenedColor = HitBox->OpenBox();

			// Print the result to the screen (debug)
			if (GEngine)
			{
				// Convert the Enum to a readable string
				FString ColorString = UEnum::GetValueAsString(OpenedColor);

				// Optional: Strip out the "EBoxColor::" prefix for a cleaner debug message
				ColorString.Split(TEXT("::"), nullptr, &ColorString);

				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("Player opened a %s box!"), *ColorString));
			}

			// Process the box's effect on the player based on the color
			ProcessMysteryBox(OpenedColor);
		}
	}
}


void APlayerCharacter::ProcessMysteryBox(EBoxColor BoxColor)
{
	// ROLL FOR THE PRESENT COLOR
	int32 BoxRoll = FMath::RandRange(1, 100);
	FString PresentColor = "";

	if (BoxColor == EBoxColor::Green)
	{
		if (BoxRoll <= 70) PresentColor = "Green";
		else if (BoxRoll <= 90) PresentColor = "Yellow";
		else PresentColor = "Red";
	}
	else if (BoxColor == EBoxColor::Yellow)
	{
		if (BoxRoll <= 10) PresentColor = "Green";
		else if (BoxRoll <= 90) PresentColor = "Yellow";
		else PresentColor = "Red";
	}
	else if (BoxColor == EBoxColor::Red)
	{
		if (BoxRoll <= 10) PresentColor = "Green";
		else if (BoxRoll <= 30) PresentColor = "Yellow";
		else PresentColor = "Red";
	}

	// RESOLVE YELLOW PRESENTS (50/50 split into Green or Red)
	if (PresentColor == "Yellow")
	{
		int32 YellowRoll = FMath::RandRange(1, 100);
		PresentColor = (YellowRoll <= 50) ? "Green" : "Red";

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Got a Yellow Present! Flipping coin..."));
	}

	// ROLL FOR THE FINAL EFFECT
	int32 EffectRoll = FMath::RandRange(1, 100);

	if (PresentColor == "Green")
	{
		// GREEN PRESENT EFFECTS
		if (EffectRoll <= 20)
		{
			// 20% Chance: Fragment
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RESULT: Found a Fragment!"));
			// FragmentCount++; (will implement GameMode tracking later)
		}
		else if (EffectRoll <= 60)
		{
			// 40% Chance: Speed Up 3s
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RESULT: Speed Up for 3 Seconds!"));
			ApplySpeedModifier(1.5f, 3.0f); // Example: 50% faster
		}
		else
		{
			// 40% Chance: Enemy Stun 2s
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RESULT: Stunned the Enemy for 2 Seconds!"));
			// will tell the GameMode to stun the other player later
		}
	}
	else
	{
		// RED PRESENT EFFECTS
		if (EffectRoll <= 20)
		{
			// 20% Chance: Enemy gets a Fragment
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RESULT: Enemy got a Fragment!"));
			// will tell the GameMode to give the enemy a fragment later
		}
		else if (EffectRoll <= 60)
		{
			// 40% Chance: Speed Down 3s
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RESULT: Speed Down for 3 Seconds!"));
			ApplySpeedModifier(0.5f, 3.0f); // Example: 50% slower
		}
		else
		{
			// 40% Chance: Self Stun 2s
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RESULT: Self Stun for 2 Seconds!"));
			ApplyStun(2.0f);
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

