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
};
