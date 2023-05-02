
#include "LevelSegment.h"
#include "Obstacle.h"
#include "PickupBase.h"
#include "Components/BoxComponent.h"

ALevelSegment::ALevelSegment()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	RootComponent = BoxCollider;
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	EndLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EndLocationComponent"));
	EndLocationComponent->SetupAttachment(BoxCollider);

	Obstacles.MaxObstacleAmount = 10;
}

void ALevelSegment::BeginPlay()
{
	Super::BeginPlay();

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

	for(int i = 0; i < Obstacles.SpawnedObstacles.Num(); i++)
	{
		Obstacles.SpawnedObstacles[i]->SetActorLocation(GetRandomLocationWithinSegmentBounds());
		Obstacles.SpawnedObstacles[i]->ShowSelf();
	}

	if(Obstacles.SpawnedObstacles.Num() >= Obstacles.MaxObstacleAmount) return;
	
	while(Obstacles.SpawnedObstacles.Num() < AmountOfObstacles)
	{
		FActorSpawnParameters SpawnParams;
		int RandomObstacleIndex = FMath::RandRange(0, (Obstacles.ObstacleBlueprints.Num() - 1));
		TObjectPtr<AObstacle> SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(Obstacles.ObstacleBlueprints[RandomObstacleIndex], GetActorLocation(), GetActorRotation(), SpawnParams);
		SpawnedObstacle->OnObstacleDodged.AddDynamic(this, &ALevelSegment::OnObstacleDodged);
		SpawnedObstacle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		Obstacles.SpawnedObstacles.Add(SpawnedObstacle);
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
		Pickups.SpawnedPickups[i]->ShowSelf();
	}
}

FVector ALevelSegment::GetRandomLocationWithinSegmentBounds()
{
	float SpawnX = FMath::RandRange(GetActorLocation().X, GetEndLocation().X);
	float SpawnY = GetActorLocation().Y + (300.f * FMath::RandRange(-1, 1));
	
	FVector RandomPos = FVector(SpawnX, SpawnY, GetActorLocation().Z);

	return RandomPos;
}

void ALevelSegment::OnObstacleDodged()
{
	float Chance = FMath::RandRange(0.f, 1.f);

	if(Chance <= Obstacles.ChanceForFurtherObstacleToExplodeWhenOneIsDodged)
	{
		TObjectPtr<AObstacle> ObstacleToExplode;
		int RetryAmount = 5;
		//find an un-hit and un-exploded obstacle
		do
		{
			int RandomSpawnedObstacleIndex = FMath::RandRange(0, Obstacles.SpawnedObstacles.Num() - 1);
			ObstacleToExplode = Obstacles.SpawnedObstacles[RandomSpawnedObstacleIndex];
			
			RetryAmount--;
			if(RetryAmount <= 0) return;
		}
		while (ObstacleToExplode->bIsSelfHidden);

		ObstacleToExplode->Explode();
	}
}
