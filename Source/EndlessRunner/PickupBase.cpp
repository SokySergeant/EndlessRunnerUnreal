
#include "PickupBase.h"
#include "EndlessRunnerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

APickupBase::APickupBase()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootSceneComponent->SetupAttachment(RootComponent);
	
    BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxColliderComponent");
	BoxCollider->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnBoxColliderOverlap);
	BoxCollider->SetupAttachment(RootSceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetupAttachment(BoxCollider);
}

void APickupBase::OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	TObjectPtr<AEndlessRunnerCharacter> Player = Cast<AEndlessRunnerCharacter>(OtherActor);
	
	if(Player)
	{
		OnPickup(Player);
		SetActorHiddenInGame(true);
	}
}

void APickupBase::OnPickup(TObjectPtr<AEndlessRunnerCharacter> Player)
{
	
}
