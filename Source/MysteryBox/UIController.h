#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIController.generated.h"

UCLASS()
class MYSTERYBOX_API AUIController : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AUIController();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void UpdatePlayerScore(int32 playerId);
    void CallWinScreen(int32 playerId);

private:

    UPROPERTY()
    APlayerController* PC;
    
    int P1Score = 0;
    int P2Score = 0;

    char winningText[16];
    bool gameOver = false;

    void Player1UI();
    void Player2UI();

    void WinningUI();
};