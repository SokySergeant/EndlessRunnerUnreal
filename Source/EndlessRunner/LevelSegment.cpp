
#include "LevelSegment.h"
#include "MyGameInstance.h"
#include "Obstacle.h"
#include "Components/BoxComponent.h"

ALevelSegment::ALevelSegment()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	EndLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EndLocationComponent"));
	EndLocationComponent->SetupAttachment(BoxCollider);
}

void ALevelSegment::BeginPlay()
{
	Super::BeginPlay();

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	SetupSpawnables(1.f);
}

void ALevelSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelSegment::SetupSpawnables(float Difficulty)
{
	int numberOfObstacles = FGenericPlatformMath::RoundToInt(Difficulty);
	UE_LOG(LogTemp, Warning, TEXT("%d"), numberOfObstacles);

	if(Obstacles.IsEmpty()) return;

	while(SpawnedObstacles.Num() < numberOfObstacles)
	{
		FActorSpawnParameters SpawnParams;
		int RandomSegmentIndex = FMath::RandRange(0, (Obstacles.Num() - 1));
		TObjectPtr<AObstacle> SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(Obstacles[RandomSegmentIndex].ObstacleBlueprint, GetActorLocation(), GetActorRotation(), SpawnParams);
		SpawnedObstacle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SpawnedObstacles.Add(SpawnedObstacle);
	}
	
	for(int i = 0; i < SpawnedObstacles.Num(); i++)
	{
		SpawnedObstacles[i]->SetActorLocation(GetRandomLocationWithinSegmentBounds());
		SpawnedObstacles[i]->SetActorHiddenInGame(false);
	}
}

FVector ALevelSegment::GetRandomLocationWithinSegmentBounds()
{
	float SpawnX = FMath::RandRange(GetActorLocation().X, GetEndLocation().X);
	float SpawnY = MyGameInstance->GetRandomLaneYValue();
	FVector RandomPos = FVector(SpawnX, SpawnY, GetActorLocation().Z);

	return RandomPos;
}

