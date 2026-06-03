#include "BoxManager.h"
#include "MysteryBoxActor.h"
#include "Engine/World.h"

ABoxManager::ABoxManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Default Grid Values (e.g., a 3x5 grid = 15 boxes)
	Rows = 3;
	Columns = 5;
	SpacingX = 250.0f;
	SpacingY = 250.0f;
}


void ABoxManager::BeginPlay()
{
	Super::BeginPlay();

	GenerateGrid();
}


void ABoxManager::GenerateGrid()
{
	// Safety check: Make sure we assigned a box class in the editor
	if (!BoxClass)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BoxManager: BoxClass is null! Assign it in the Blueprint."));
		return;
	}

	FVector ManagerLocation = GetActorLocation();

	// Calculate the offset required to center the grid on this actor
	// Total width/height is (Count - 1) * Spacing
	float StartX = -((Columns - 1) * SpacingX) / 2.0f;
	float StartY = -((Rows - 1) * SpacingY) / 2.0f;

	for (int32 r = 0; r < Rows; ++r)
	{
		for (int32 c = 0; c < Columns; ++c)
		{
			// Calculate the specific location for this box
			float CurrentX = StartX + (c * SpacingX);
			float CurrentY = StartY + (r * SpacingY);

			FVector SpawnLocation = ManagerLocation + FVector(CurrentX, CurrentY, 0.0f);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			// Spawn the box
			AMysteryBoxActor* NewBox = GetWorld()->SpawnActor<AMysteryBoxActor>(BoxClass, SpawnLocation, SpawnRotation, SpawnParams);

			if (NewBox)
			{
				SpawnedBoxes.Add(NewBox);
			}
		}
	}
}