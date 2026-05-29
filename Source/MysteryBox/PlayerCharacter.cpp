// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"

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

	// Add Input Mapping Context (Only applies if possessed by a local player)
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
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

	// Placeholder for Box Interaction
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Interact Button Pressed"));
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

