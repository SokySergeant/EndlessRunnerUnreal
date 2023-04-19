
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.generated.h"

class UBoxComponent;
class AObstacle;
class UMyGameInstance;

USTRUCT()
struct FObstacleStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AObstacle> ObstacleBlueprint;

	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", UIMax = "1.0"))
	float Frequency;
};

UCLASS()
class ENDLESSRUNNER_API ALevelSegment : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollider;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EndLocationComponent;

	UPROPERTY(EditAnywhere, Category = Spawnables)
	TArray<FObstacleStruct> Obstacles;

	TArray<TObjectPtr<AActor>> SpawnedObstacles;
	
	TObjectPtr<UMyGameInstance> MyGameInstance;
	
public:	
	ALevelSegment();

protected:
	virtual void BeginPlay() override;

	FVector GetRandomLocationWithinSegmentBounds();

public:	
	virtual void Tick(float DeltaTime) override;

	FVector GetEndLocation() {return EndLocationComponent->GetComponentLocation();};

	void SetupSpawnables(float Difficulty);
};
