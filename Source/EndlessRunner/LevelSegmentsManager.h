
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSegmentsManager.generated.h"

class ALevelSegment;
class UBoxComponent;
class UInGameWidget;

UCLASS()
class ENDLESSRUNNER_API ALevelSegmentsManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelSegments)
	float ScrollSpeed;
	
	UPROPERTY(EditAnywhere, Category = LevelSegments)
	float ScrollSpeedIncreaseRate;

	UPROPERTY(VisibleAnywhere, Category = LevelSegments)
	float Difficulty;
	
	UPROPERTY(EditAnywhere, Category = LevelSegments)
	float DifficultyIncreaseRate;

	UPROPERTY(EditAnywhere, Category = LevelSegments)
	float ScoreMultiplier;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollider;
	
	TArray<TObjectPtr<ALevelSegment>> CurrentlyActiveSegments;
	
public:	
	ALevelSegmentsManager();

	UPROPERTY(EditAnywhere, Category = LevelSegments)
	TArray<TSubclassOf<ALevelSegment>> SegmentBlueprints;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInGameWidget> InGameWidget;

protected:
	virtual void BeginPlay() override;

	void SpawnLevelSegment(FVector SpawnPos);
	
	UFUNCTION()
	void OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	TObjectPtr<ALevelSegment> GetFurthestLevelSegment();

	UFUNCTION()
	void StartMovingSegments();
	UFUNCTION()
	void StopMovingSegments();
	bool bCanMoveSegments;

	void UpdateInGameWidgetValues();

public:	
	virtual void Tick(float DeltaTime) override;

};
