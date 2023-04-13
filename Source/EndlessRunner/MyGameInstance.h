
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ENDLESSRUNNER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	float Lane1YValue = -300.f;
	float Lane2YValue = 0.f;
	float Lane3YValue = 300.f;
};
