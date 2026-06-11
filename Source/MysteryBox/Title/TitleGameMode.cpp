// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"


 ATitleGameMode::ATitleGameMode() 
 {

 }

 void ATitleGameMode::BeginPlay()
 {
    Super::BeginPlay();
    
    UWorld* const World = GetWorld();
	UE_LOG(LogTemp, Log, TEXT("GameMode Started..."));
	if (World)
    {
        // Setup game controller
        TitleController = World->SpawnActor<ATitleController>();

        if (TitleController)
        {
            // Further configuration can be done here after spawning
            UE_LOG(LogTemp, Log, TEXT("TitleController Successfully instantiated..."));
        }else UE_LOG(LogTemp, Log, TEXT("Couldnt spawn TitleController..."));
		
    }else UE_LOG(LogTemp, Log, TEXT("No World Found..."));
 }