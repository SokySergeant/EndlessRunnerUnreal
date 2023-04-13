
#pragma once

#include "CoreMinimal.h"
#include "LevelSegment.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LevelSegmentsManager.generated.h"

UCLASS()
class ENDLESSRUNNER_API ALevelSegmentsManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = LevelSegments)
	float ScrollSpeed;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollider;
	
	TArray<TObjectPtr<ALevelSegment>> CurrentlyActiveSegments;
	
public:	
	ALevelSegmentsManager();

	UPROPERTY(EditAnywhere, Category = LevelSegments)
	TArray<TSubclassOf<ALevelSegment>> SegmentBlueprints;

protected:
	virtual void BeginPlay() override;

	void SpawnLevelSegment(FVector spawnPos);
	
	UFUNCTION()
	void OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	TObjectPtr<ALevelSegment> GetFurthestLevelSegment();

public:	
	virtual void Tick(float DeltaTime) override;

};
