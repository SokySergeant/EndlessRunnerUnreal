
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.generated.h"

class UBoxComponent;
class AObstacle;
class UMyGameInstance;
class APickupBase;

USTRUCT()
struct FObstaclesStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AObstacle>> ObstacleBlueprints;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AObstacle>> SpawnedObstacles;

	UPROPERTY(EditAnywhere)
	float FrequencyOfObstacles;

	UPROPERTY(EditAnywhere)
	int MaxObstacleAmount;

	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", UIMax = "1.0"))
	float ChanceForFurtherObstacleToExplodeWhenOneIsDodged; //I'm sorry, I couldn't find a shorter name
};

USTRUCT()
struct FPickupsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickupBase>> PickupBlueprints;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<APickupBase>> SpawnedPickups;

	UPROPERTY(EditAnywhere)
	float FrequencyOfPickups;
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
	FObstaclesStruct Obstacles;

	UPROPERTY(EditAnywhere, Category = Spawnables)
	FPickupsStruct Pickups;
	
public:	
	ALevelSegment();

protected:
	virtual void BeginPlay() override;

	FVector GetRandomLocationWithinSegmentBounds();

	void SetupObstacles(int AmountOfObstacles);
	void SetupPickups(int AmountOfPickups);

	UFUNCTION()
	void OnObstacleDodged();

public:	
	FVector GetEndLocation() {return EndLocationComponent->GetComponentLocation();};

	void SetupSpawnables(float Difficulty);
};
