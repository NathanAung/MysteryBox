// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MysteryBoxActor.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

UENUM(BlueprintType)
enum class EBoxColor : uint8
{
	Green UMETA(DisplayName = "Green"),
	Yellow UMETA(DisplayName = "Yellow"),
	Red UMETA(DisplayName = "Red")
};

UCLASS()
class MYSTERYBOX_API AMysteryBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMysteryBoxActor();

protected:
	virtual void BeginPlay() override;

	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BoxMesh;

	// --- Materials ---
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInterface* GreenMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInterface* YellowMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInterface* RedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInterface* CooldownMaterial; // What the box looks like when opened

	// --- Timers & State ---
	FTimerHandle ColorCycleTimerHandle;
	FTimerHandle CooldownTimerHandle;

	void CycleColor();
	void ResetCooldown();
	void UpdateMaterial();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box State")
	EBoxColor CurrentColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box State")
	bool bIsOnCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Box Settings")
	float ColorCycleRate; // How fast the colors change

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Box Settings")
	float CooldownDuration; // Standard 10 seconds

	// Returns the color it was when opened. Called by the Player Character.
	EBoxColor OpenBox();
};
