#pragma once


#include "CoreMinimal.h"



namespace PUTILS{

    AActor* GetActorByName(FString actorName, UWorld* world);

    TArray<AActor*> GetActorsByTag(FName tag, UWorld* world);
    AActor* GetFirstActorByTag(FName tag, UWorld* world);
}
