
#include "HealthPickup.h"
#include "EndlessRunnerCharacter.h"

void AHealthPickup::OnPickup(TObjectPtr<AEndlessRunnerCharacter> Player)
{
	Player->UpdateHealthBy(1);
}
