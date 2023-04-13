
#include "LevelSegmentsManager.h"

ALevelSegmentsManager::ALevelSegmentsManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALevelSegmentsManager::OnBoxColliderOverlap);

	ScrollSpeed = 10.f;
}

void ALevelSegmentsManager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnLevelSegment(GetActorLocation() + FVector(3000.f, 0.f, 0.f));
	SpawnLevelSegment(CurrentlyActiveSegments[0]->GetEndLocation());
	SpawnLevelSegment(CurrentlyActiveSegments[1]->GetEndLocation());
}

void ALevelSegmentsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(int i = 0; i < CurrentlyActiveSegments.Num(); i++)
	{
		CurrentlyActiveSegments[i]->AddActorLocalOffset(FVector(-ScrollSpeed, 0, 0));
	}
}

void ALevelSegmentsManager::SpawnLevelSegment(FVector spawnPos)
{
	FActorSpawnParameters SpawnParams;
	FRotator SpawnRot = GetActorRotation();
	int RandomSegmentIndex = FMath::RandRange(0, (SegmentBlueprints.Num() - 1));
	CurrentlyActiveSegments.Add(GetWorld()->SpawnActor<ALevelSegment>(SegmentBlueprints[RandomSegmentIndex], spawnPos, SpawnRot, SpawnParams));
}

void ALevelSegmentsManager::OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(OtherActor->IsA(ALevelSegment::StaticClass()))
	{
		OtherActor->SetActorLocation(GetFurthestLevelSegment()->GetEndLocation());
	}
}

TObjectPtr<ALevelSegment> ALevelSegmentsManager::GetFurthestLevelSegment()
{
	TObjectPtr<ALevelSegment> furthestLevelSegment = CurrentlyActiveSegments[0];
	
	for(int i = 0; i < CurrentlyActiveSegments.Num(); i++)
	{
		if(CurrentlyActiveSegments[i]->GetActorLocation().X > furthestLevelSegment->GetActorLocation().X)
		{
			furthestLevelSegment = CurrentlyActiveSegments[i];
		}
	}

	return furthestLevelSegment;
}
