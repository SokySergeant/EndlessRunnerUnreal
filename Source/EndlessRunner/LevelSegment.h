
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
	TArray<TObjectPtr<AActor>> SpawnedObstacles;

	UPROPERTY(EditAnywhere)
	float FrequencyOfObstacles;
};

USTRUCT()
struct FPickupsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APickupBase>> PickupBlueprints;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AActor>> SpawnedPickups;

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
	
	TObjectPtr<UMyGameInstance> MyGameInstance;
	
public:	
	ALevelSegment();

protected:
	virtual void BeginPlay() override;

	FVector GetRandomLocationWithinSegmentBounds();

	void SetupObstacles(int AmountOfObstacles);
	void SetupPickups(int AmountOfPickups);

public:	
	FVector GetEndLocation() {return EndLocationComponent->GetComponentLocation();};

	void SetupSpawnables(float Difficulty);
};
