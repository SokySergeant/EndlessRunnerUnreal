
#include "LevelSegment.h"
#include "MyGameInstance.h"
#include "Obstacle.h"
#include "PickupBase.h"
#include "Components/BoxComponent.h"

ALevelSegment::ALevelSegment()
{
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

void ALevelSegment::SetupSpawnables(float Difficulty)
{
	int AmountOfObstacles = Difficulty * Obstacles.FrequencyOfObstacles;

	SetupObstacles(AmountOfObstacles);

	if(FMath::RandRange(0, 1) >= Pickups.FrequencyOfPickups)
	{
		SetupPickups(1);
	}
}

void ALevelSegment::SetupObstacles(int AmountOfObstacles)
{
	if(Obstacles.ObstacleBlueprints.IsEmpty()) return;

	while(Obstacles.SpawnedObstacles.Num() < AmountOfObstacles)
	{
		FActorSpawnParameters SpawnParams;
		int RandomObstacleIndex = FMath::RandRange(0, (Obstacles.ObstacleBlueprints.Num() - 1));
		TObjectPtr<AObstacle> SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(Obstacles.ObstacleBlueprints[RandomObstacleIndex], GetActorLocation(), GetActorRotation(), SpawnParams);
		SpawnedObstacle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		Obstacles.SpawnedObstacles.Add(SpawnedObstacle);
	}
	
	for(int i = 0; i < Obstacles.SpawnedObstacles.Num(); i++)
	{
		Obstacles.SpawnedObstacles[i]->SetActorLocation(GetRandomLocationWithinSegmentBounds());
		Obstacles.SpawnedObstacles[i]->SetActorHiddenInGame(false);
	}
}

void ALevelSegment::SetupPickups(int AmountOfPickups)
{
	if(Pickups.PickupBlueprints.IsEmpty()) return;

	while(Pickups.SpawnedPickups.Num() < AmountOfPickups)
	{
		FActorSpawnParameters SpawnParams;
		int RandomPickupIndex = FMath::RandRange(0, (Pickups.PickupBlueprints.Num() - 1));
		TObjectPtr<APickupBase> SpawnedPickup = GetWorld()->SpawnActor<APickupBase>(Pickups.PickupBlueprints[RandomPickupIndex], GetActorLocation(), GetActorRotation(), SpawnParams);
		SpawnedPickup->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		Pickups.SpawnedPickups.Add(SpawnedPickup);
	}
	
	for(int i = 0; i < Pickups.SpawnedPickups.Num(); i++)
	{
		Pickups.SpawnedPickups[i]->SetActorLocation(GetRandomLocationWithinSegmentBounds());
		Pickups.SpawnedPickups[i]->SetActorHiddenInGame(false);
	}
}

FVector ALevelSegment::GetRandomLocationWithinSegmentBounds()
{
	float SpawnX = FMath::RandRange(GetActorLocation().X, GetEndLocation().X);
	float SpawnY = MyGameInstance->GetRandomLaneYValue();
	FVector RandomPos = FVector(SpawnX, SpawnY, GetActorLocation().Z);

	return RandomPos;
}
