
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ENDLESSRUNNER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	float Score = 0.f;
	float HighScore = 0.f;
};
