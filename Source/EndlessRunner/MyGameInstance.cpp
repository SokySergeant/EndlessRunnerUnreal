
#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
	Super::Init();
	
	LaneYValues.Add(-300.f);
	LaneYValues.Add(0.f);
	LaneYValues.Add(300.f);
}
