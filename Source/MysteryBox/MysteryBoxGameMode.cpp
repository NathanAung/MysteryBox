// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBoxGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMysteryBoxGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Player 2
	UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
}


AActor* AMysteryBoxGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// Increment the counter every time a player joins
	PlayersSpawnedCount++;

	// First player gets P1, Second player gets P2
	FString TargetTag = (PlayersSpawnedCount == 1) ? TEXT("P1") : TEXT("P2");

	// Find all Player Starts in the level
	TArray<AActor*> FoundPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPlayerStarts);

	// Loop through them and find the matching tag
	for (AActor* StartActor : FoundPlayerStarts)
	{
		if (APlayerStart* PStart = Cast<APlayerStart>(StartActor))
		{
			if (PStart->PlayerStartTag.ToString() == TargetTag)
			{
				return PStart;
			}
		}
	}

	// Fallback to default engine logic if a tag is missing
	return Super::ChoosePlayerStart_Implementation(Player);
}