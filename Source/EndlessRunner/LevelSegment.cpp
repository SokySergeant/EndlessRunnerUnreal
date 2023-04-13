
#include "LevelSegment.h"

ALevelSegment::ALevelSegment()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	EndLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EndLocationComponent"));
	EndLocationComponent->SetupAttachment(BoxCollider);
}

void ALevelSegment::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}