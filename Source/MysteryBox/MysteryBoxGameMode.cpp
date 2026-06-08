// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBoxGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "MysteryBoxActor.h"

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


void AMysteryBoxGameMode::AddFragmentToPlayer(APlayerCharacter* InstigatorPlayer, bool bAddToEnemy)
{
	if (bGameOver || !InstigatorPlayer) return;

	// By default, assume the target is the player who called the function
	APlayerCharacter* TargetPlayer = InstigatorPlayer;

	// If the boolean is true, search for the other player to make them the target
	if (bAddToEnemy)
	{
		TArray<AActor*> FoundPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayers);

		for (AActor* Actor : FoundPlayers)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
			if (Player && Player != InstigatorPlayer)
			{
				TargetPlayer = Player;
				break; // We found the enemy, stop searching
			}
		}
	}

	// Reward whoever the target ended up being
	if (TargetPlayer)
	{
		TargetPlayer->FragmentCount++;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Player has %d / 3 Fragments"), TargetPlayer->FragmentCount));

		// GAME OVER LOGIC
		if (TargetPlayer->FragmentCount >= 3)
		{	
			bGameOver = true;

			// Determine who won based on their Controller ID (0 = Player 1, 1 = Player 2)
			int32 WinningPlayerNum = 1;
			if (APlayerController* PC = Cast<APlayerController>(TargetPlayer->GetController()))
			{
				WinningPlayerNum = UGameplayStatics::GetPlayerControllerID(PC) + 1;
			}

			// Print the Winner
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Orange, FString::Printf(TEXT("PLAYER %d WINS"), WinningPlayerNum));

			// Disable all Players
			TArray<AActor*> AllPlayers;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), AllPlayers);
			for (AActor* Actor : AllPlayers)
			{
				if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
				{
					Player->DisablePlayer();
				}
			}

			// Deactivate all Boxes
			TArray<AActor*> AllBoxes;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMysteryBoxActor::StaticClass(), AllBoxes);
			for (AActor* Actor : AllBoxes)
			{
				if (AMysteryBoxActor* Box = Cast<AMysteryBoxActor>(Actor))
				{
					Box->ForceDeactivate();
				}
			}
		}
	}
}


void AMysteryBoxGameMode::StunEnemy(APlayerCharacter* InstigatorPlayer, float Duration)
{
	// Find all players in the level
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayers);

	// Loop through them to find the one that is NOT the instigator
	for (AActor* Actor : FoundPlayers)
	{
		APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(Actor);
		if (TargetPlayer && TargetPlayer != InstigatorPlayer)
		{
			TargetPlayer->ApplyStun(Duration);
			return; // Found the enemy, we can stop searching
		}
	}
}


void AMysteryBoxGameMode::ModifyEnemySpeed(APlayerCharacter* InstigatorPlayer, float Multiplier, float Duration)
{
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayers);

	for (AActor* Actor : FoundPlayers)
	{
		APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(Actor);
		if (TargetPlayer && TargetPlayer != InstigatorPlayer)
		{
			TargetPlayer->ApplySpeedModifier(Multiplier, Duration);
			return;
		}
	}
}