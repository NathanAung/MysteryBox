// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MysteryBoxGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYSTERYBOX_API AMysteryBoxGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// Overriding the default spawn logic
	// For ensuring P1 ans P2 spawn on correct sides of the map
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	// keeps track of how many spawns we've handed out
	int32 PlayersSpawnedCount = 0;
};
