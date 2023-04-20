
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

class UBoxComponent;

UCLASS()
class ENDLESSRUNNER_API AObstacle : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootSceneComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollider;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere)
	float Damage;
	
public:	
	AObstacle();

protected:
	UFUNCTION()
	void OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
};
