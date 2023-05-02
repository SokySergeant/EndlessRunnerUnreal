
#include "LevelSegmentsManager.h"
#include "EndlessRunnerCharacter.h"
#include "InGameWidget.h"
#include "LevelSegment.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ALevelSegmentsManager::ALevelSegmentsManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	RootComponent = BoxCollider;
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALevelSegmentsManager::OnBoxColliderOverlap);

	ScrollSpeed = 25.f;
	ScrollSpeedIncreaseRate = 1.f;
	DifficultyIncreaseRate = 1.f;
	ScoreMultiplier = 1.f;
}

void ALevelSegmentsManager::BeginPlay()
{
	Super::BeginPlay();
	
	Difficulty = 1.f;
	
	SpawnInitialSegments();
	StartMovingSegments();
}

void ALevelSegmentsManager::SpawnInitialSegments()
{
	SpawnLevelSegment(GetActorLocation() + FVector(3000.f, 0.f, 0.f));
	for(int i = 0; i < 20; i++)
	{
		SpawnLevelSegment(CurrentlyActiveSegments[i]->GetEndLocation());
	}
}

void ALevelSegmentsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bCanMoveSegments) return;

	MoveSegments(DeltaTime);
	UpdateInGameWidgetValues();
}

void ALevelSegmentsManager::MoveSegments(float Increment)
{
	Difficulty += Increment * DifficultyIncreaseRate;
	ScrollSpeed += Increment * ScrollSpeedIncreaseRate;

	for(int i = 0; i < CurrentlyActiveSegments.Num(); i++)
	{
		CurrentlyActiveSegments[i]->AddActorLocalOffset(FVector(-ScrollSpeed, 0, 0));
	}
}

void ALevelSegmentsManager::SpawnLevelSegment(FVector SpawnPos)
{
	if(SegmentBlueprints.IsEmpty()) return;

	FActorSpawnParameters SpawnParams;
	FRotator SpawnRot = GetActorRotation();
	int RandomSegmentIndex = FMath::RandRange(0, (SegmentBlueprints.Num() - 1));
	TObjectPtr<ALevelSegment> TempSegment = GetWorld()->SpawnActor<ALevelSegment>(SegmentBlueprints[RandomSegmentIndex], SpawnPos, SpawnRot, SpawnParams);
	CurrentlyActiveSegments.Add(TempSegment);
}

void ALevelSegmentsManager::OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(OtherActor->IsA(ALevelSegment::StaticClass()))
	{
		TObjectPtr<ALevelSegment> TempSegment = Cast<ALevelSegment>(OtherActor);
		TempSegment->SetupSpawnables(Difficulty);
		OtherActor->SetActorLocation(GetFurthestLevelSegment()->GetEndLocation());
	}
}

TObjectPtr<ALevelSegment> ALevelSegmentsManager::GetFurthestLevelSegment()
{
	TObjectPtr<ALevelSegment> FurthestLevelSegment = CurrentlyActiveSegments[0];
	
	for(int i = 0; i < CurrentlyActiveSegments.Num(); i++)
	{
		if(CurrentlyActiveSegments[i]->GetActorLocation().X > FurthestLevelSegment->GetActorLocation().X)
		{
			FurthestLevelSegment = CurrentlyActiveSegments[i];
		}
	}

	return FurthestLevelSegment;
}

void ALevelSegmentsManager::StartMovingSegments()
{
	bCanMoveSegments = true;
}

void ALevelSegmentsManager::StopMovingSegments()
{
	bCanMoveSegments = false;
}

void ALevelSegmentsManager::BindToOnPlayerDeath()
{
	MyPlayer->OnPlayerDeath.AddDynamic(this, &ALevelSegmentsManager::StopMovingSegments);
}

void ALevelSegmentsManager::UpdateInGameWidgetValues()
{
	if(!InGameWidget) return;
	
	float Score = (ScrollSpeed + Difficulty) * ScoreMultiplier;

	if(MyPlayer->bIsFirstPlayer)
	{
		InGameWidget->P1ScoreText->SetText(FText::AsNumber(Score));
		InGameWidget->P1SpeedText->SetText(FText::AsNumber(ScrollSpeed));
	}else
	{
		InGameWidget->P2ScoreText->SetText(FText::AsNumber(Score));
		InGameWidget->P2SpeedText->SetText(FText::AsNumber(ScrollSpeed));
	}
}

void ALevelSegmentsManager::SaveHighScore(int PlayerNum)
{
	if(PlayerNum == 0)
	{
		//save high score to p1
	}else if(PlayerNum == 1)
	{
		//save high score to p2
	}
}
