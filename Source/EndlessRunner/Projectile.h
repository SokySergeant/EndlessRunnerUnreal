
#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Projectile.generated.h"

UCLASS()
class ENDLESSRUNNER_API AProjectile : public AObstacle
{
	GENERATED_BODY()
	
public:
	AProjectile();

	virtual void ResetSelf() override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed;

	FVector RelativeLocationOffset;
};
