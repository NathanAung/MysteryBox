// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBoxGameMode.h"
#include "Kismet/GameplayStatics.h"

void AMysteryBoxGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Player 2
	UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
}

