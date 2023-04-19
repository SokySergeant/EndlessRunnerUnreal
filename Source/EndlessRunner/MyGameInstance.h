
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ENDLESSRUNNER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	TArray<float> LaneYValues;

	float Score = 0.f;
	float HighScore = 0.f;

	float GetRandomLaneYValue(){ return LaneYValues[FMath::RandRange(0, LaneYValues.Num() - 1)]; };

protected:
	void Init() override;
};
