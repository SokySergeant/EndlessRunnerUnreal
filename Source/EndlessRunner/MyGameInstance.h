
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ENDLESSRUNNER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	float P1HighScore = 0.f;
	float P2HighScore = 0.f;
};
