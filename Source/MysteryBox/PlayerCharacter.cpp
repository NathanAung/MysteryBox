// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "MysteryBoxActor.h"
#include "MysteryBoxGameMode.h"


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

	// Get a reference to our GameMode
	AMysteryBoxGameMode* GM = Cast<AMysteryBoxGameMode>(GetWorld()->GetAuthGameMode());

	if (PresentColor == "Green")
	{
		// GREEN PRESENT EFFECTS
		if (EffectRoll <= 20)
		{
			// 20% Chance: Fragment
			if (GM) GM->AddFragmentToPlayer(this, false);
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RESULT: Found a Fragment!"));
		}
		else if (EffectRoll <= 60)
		{
			// 40% Chance: Speed Up
			ApplySpeedModifier(1.5f, 3.0f); // 50% faster, 3 seconds
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RESULT: Speed Up for 3 Seconds!"));
		}
		else
		{
			// 40% Chance: Enemy Stun
			if (GM) GM->StunEnemy(this, 2.0f);
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("RESULT: Stunned the Enemy for 2 Seconds!"));
		}
	}
	else
	{
		// RED PRESENT EFFECTS
		if (EffectRoll <= 20)
		{
			// 20% Chance: Enemy gets a Fragment
			if (GM) GM->AddFragmentToPlayer(this, true);
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RESULT: Enemy got a Fragment!"));
		}
		else if (EffectRoll <= 60)
		{
			// 40% Chance: Speed Down
			ApplySpeedModifier(0.5f, 3.0f); // 50% slower, 3 seconds
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RESULT: Speed Down for 3 Seconds!"));
		}
		else
		{
			// 40% Chance: Self Stun
			ApplyStun(2.0f);
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RESULT: Self Stun for 2 Seconds!"));
		}
	}
}


// STATUS EFFECTS
void APlayerCharacter::ApplySpeedModifier(float Multiplier, float Duration)
{
	// Change the speed
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed * Multiplier;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Cyan, FString::Printf(TEXT("Speed changed to %f"), GetCharacterMovement()->MaxWalkSpeed));

	// Clear any existing timer so a new buff overwrites an old one safely
	GetWorldTimerManager().ClearTimer(SpeedTimerHandle);

	// Start the timer to reset the speed
	GetWorldTimerManager().SetTimer(SpeedTimerHandle, this, &APlayerCharacter::ResetSpeed, Duration, false);
}


void APlayerCharacter::ResetSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Speed Returned to Normal"));
}


void APlayerCharacter::ApplyStun(float Duration)
{
	// This completely disables the Move() and Interact() functions
	bIsStunned = true;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, TEXT("Stun Triggered"));

	// Safely manage the timer
	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &APlayerCharacter::ResetStun, Duration, false);
}


void APlayerCharacter::ResetStun()
{
	bIsStunned = false;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Stun Ended"));
}


void APlayerCharacter::DisablePlayer()
{
	bIsStunned = true;

	// Clear timers so a previously running buff/debuff doesn't accidentally reset the stun
	GetWorldTimerManager().ClearTimer(SpeedTimerHandle);
	GetWorldTimerManager().ClearTimer(StunTimerHandle);

	// Force their speed to zero just to be completely safe
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}


void APlayerCharacter::SetModelMaterial(UMaterialInterface* NewMaterial)
{
	if (!NewMaterial) return;

	// Get every mesh component attached to this character
	TArray<UMeshComponent*> AllMeshes;
	GetComponents<UMeshComponent>(AllMeshes);

	// Loop through them to find the one explicitly named "Model"
	for (UMeshComponent* MeshComp : AllMeshes)
	{
		if (MeshComp->GetName() == TEXT("boxmover"))
		{
			MeshComp->SetMaterial(0, NewMaterial);
			return; // We found it and painted it, so we can stop searching
		}
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Could not find a component named 'boxmover'"));
}