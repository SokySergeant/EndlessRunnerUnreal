
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

UCLASS()
class ENDLESSRUNNER_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();

	UPROPERTY(EditAnywhere)
	float P1HighScore;
	
	UPROPERTY(EditAnywhere)
	float P2HighScore;
};
