// Fill out your copyright notice in the Description page of Project Settings.


#include "MysteryBoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AMysteryBoxActor::AMysteryBoxActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup Mesh Component
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;

	// Ensure the mesh blocks the visibility trace from the player's interact function
	BoxMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Default Settings
	CurrentColor = EBoxColor::Green;
	bIsOnCooldown = false;
	ColorCycleRate = 1.5f; // Colors change every 1.5 seconds
	CooldownDuration = 10.0f; // 10-second cooldown rule
}


void AMysteryBoxActor::BeginPlay()
{
	Super::BeginPlay();

	// Start the initial color cycle loop
	GetWorldTimerManager().SetTimer(ColorCycleTimerHandle, this, &AMysteryBoxActor::CycleColor, ColorCycleRate, true);
	UpdateMaterial();
}


void AMysteryBoxActor::CycleColor()
{
	if (bIsOnCooldown) return;

	// Shift the color to the next state
	switch (CurrentColor)
	{
	case EBoxColor::Green:
		CurrentColor = EBoxColor::Yellow;
		break;
	case EBoxColor::Yellow:
		CurrentColor = EBoxColor::Red;
		break;
	case EBoxColor::Red:
		CurrentColor = EBoxColor::Green;
		break;
	}

	UpdateMaterial();
}


void AMysteryBoxActor::UpdateMaterial()
{
	if (bIsOnCooldown)
	{
		if (CooldownMaterial) BoxMesh->SetMaterial(0, CooldownMaterial);
		return;
	}

	switch (CurrentColor)
	{
	case EBoxColor::Green:
		if (GreenMaterial) BoxMesh->SetMaterial(0, GreenMaterial);
		break;
	case EBoxColor::Yellow:
		if (YellowMaterial) BoxMesh->SetMaterial(0, YellowMaterial);
		break;
	case EBoxColor::Red:
		if (RedMaterial) BoxMesh->SetMaterial(0, RedMaterial);
		break;
	}
}


EBoxColor AMysteryBoxActor::OpenBox()
{
	// If it's already open, do nothing and just return the current color safely
	if (bIsOnCooldown) return CurrentColor;

	bIsOnCooldown = true;

	// Stop the color cycling
	GetWorldTimerManager().ClearTimer(ColorCycleTimerHandle);

	// Set to grayscale/cooldown material
	UpdateMaterial();

	// Start the 10-second cooldown timer
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AMysteryBoxActor::ResetCooldown, CooldownDuration, false);

	// Return the color the box was at the exact moment it was opened
	return CurrentColor;
}


void AMysteryBoxActor::ResetCooldown()
{
	bIsOnCooldown = false;

	// Restart the color cycling loop
	GetWorldTimerManager().SetTimer(ColorCycleTimerHandle, this, &AMysteryBoxActor::CycleColor, ColorCycleRate, true);

	// Force an immediate visual update back to the current active color
	UpdateMaterial();
}

