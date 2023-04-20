
#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "HealthPickup.generated.h"

UCLASS()
class ENDLESSRUNNER_API AHealthPickup : public APickupBase
{
	GENERATED_BODY()

protected:
	virtual void OnPickup(TObjectPtr<AEndlessRunnerCharacter> Player) override;
};
