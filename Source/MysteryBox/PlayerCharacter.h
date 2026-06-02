// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

enum class EBoxColor : uint8;

UCLASS()
class MYSTERYBOX_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	void Move(const FInputActionValue& Value);
	void Interact(); // Placeholder

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Status")
	int32 FragmentCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Status")
	bool bIsStunned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Status")
	float BaseSpeed;

	// Placeholder functions
	void ApplySpeedModifier(float Multiplier, float Duration);
	void ApplyStun(float Duration);

private:
	// Timer Handles for resetting effects
	FTimerHandle SpeedTimerHandle;
	FTimerHandle StunTimerHandle;

	void ResetSpeed();
	void ResetStun();

	void ProcessMysteryBox(EBoxColor BoxColor);
};
