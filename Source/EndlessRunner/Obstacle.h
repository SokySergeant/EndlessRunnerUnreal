
#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "Obstacle.generated.h"

class UBoxComponent;

UCLASS()
class ENDLESSRUNNER_API AObstacle : public APickupBase
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObstacleDodgedDelegate);
public:
	FOnObstacleDodgedDelegate OnObstacleDodged;

	void Explode();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> StaggeredBoxCollider;

	UFUNCTION()
	void OnStaggeredBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ExplosionParticleSystem;

protected:
	virtual void OnPickup(TObjectPtr<AEndlessRunnerCharacter> Player) override;

	UPROPERTY(EditAnywhere)
	bool bCanTriggerExplosionInOtherObstacle;

public:
	AObstacle();
};
