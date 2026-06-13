// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "UIController.h"

#include "MysteryBoxGameMode.generated.h"

class APlayerCharacter;

UCLASS()
class MYSTERYBOX_API AMysteryBoxGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// Overriding the default spawn logic
	// For ensuring P1 ans P2 spawn on correct sides of the map
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player Setup")
	class UMaterialInterface* Player2Mat;

private:
	// keeps track of how many spawns we've handed out
	int32 PlayersSpawnedCount = 0;

	AUIController* UIController;

public:
	// Adds a fragment and checks if this player just won the game
	void AddFragmentToPlayer(APlayerCharacter* InstigatorPlayer, bool bAddToEnemy);

	// Finds the other player and applies an effect
	void StunEnemy(APlayerCharacter* InstigatorPlayer, float Duration);
	void ModifyEnemySpeed(APlayerCharacter* InstigatorPlayer, float Multiplier, float Duration);

	bool bGameOver = false;
};
