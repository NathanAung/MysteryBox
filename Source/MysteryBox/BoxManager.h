// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxManager.generated.h"

class AMysteryBoxActor;

UCLASS()
class MYSTERYBOX_API ABoxManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// --- Grid Settings ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Setup")
	TSubclassOf<AMysteryBoxActor> BoxClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Setup", meta = (ClampMin = "1"))
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Setup", meta = (ClampMin = "1"))
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Setup")
	float SpacingX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Setup")
	float SpacingY;

	// --- Box Tracking ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid State")
	TArray<AMysteryBoxActor*> SpawnedBoxes;

	void GenerateGrid();

};
