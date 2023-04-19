
#include "Obstacle.h"
#include "EndlessRunnerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootSceneComponent->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxColliderComponent");
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnBoxColliderOverlap);
	BoxCollider->SetupAttachment(RootSceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetupAttachment(BoxCollider);

	Damage = 1;
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		TObjectPtr<AEndlessRunnerCharacter> player = Cast<AEndlessRunnerCharacter>(OtherActor);
		player->UpdateHealthBy(-Damage);
		SetActorHiddenInGame(true);
	}
}
