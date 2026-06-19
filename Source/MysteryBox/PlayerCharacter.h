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
	UInputMappingContext* MappingContext_P1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MappingContext_P2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	void Move(const FInputActionValue& Value);
	void Interact();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Status")
	int32 FragmentCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Status")
	bool bIsStunned;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Status")
	float BaseSpeed;

	// Effects applied by boxes
	void ApplySpeedModifier(float Multiplier, float Duration);
	void ApplyStun(float Duration);

	// Permanently disables the player at the end of the game
	void DisablePlayer();

	// Finds the custom "Model" component and changes its material
	void SetModelMaterial(class UMaterialInterface* NewMaterial);

	UPROPERTY(EditAnywhere, Category="Audio")
	USoundBase* BoxOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* FragmentSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* EnemyFragmentSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* SpeedUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* SpeedDownSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* StunSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	float EffectSoundDelay = 0.4f;

private:
	// Timer Handles for resetting effects
	FTimerHandle SpeedTimerHandle;
	FTimerHandle StunTimerHandle;

	void ResetSpeed();
	void ResetStun();

	void ProcessMysteryBox(EBoxColor BoxColor);

	void PlayDelayedSound(USoundBase* Sound);
};
