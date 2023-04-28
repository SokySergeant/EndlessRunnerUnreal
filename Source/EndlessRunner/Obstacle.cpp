
#include "Obstacle.h"
#include "EndlessRunnerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AObstacle::AObstacle()
{
	StaggeredBoxCollider = CreateDefaultSubobject<UBoxComponent>("StaggeredBoxColliderComponent");
	StaggeredBoxCollider->SetBoxExtent(FVector(100.f, 1200.f, 500.f));
	StaggeredBoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	StaggeredBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnStaggeredBoxColliderOverlap);
	StaggeredBoxCollider->SetupAttachment(RootSceneComponent);
	StaggeredBoxCollider->AddRelativeLocation(FVector(100.f, 0.f, 0.f));
}


void AObstacle::OnPickup(TObjectPtr<AEndlessRunnerCharacter> Player)
{
	Player->UpdateHealthBy(-Damage);
}

void AObstacle::OnStaggeredBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	TObjectPtr<AEndlessRunnerCharacter> Player = Cast<AEndlessRunnerCharacter>(OtherActor);
	
	if(Player)
	{
		if(IsHidden()) return; //obstacle was hit

		if(OnObstacleDodged.IsBound())
		{
			OnObstacleDodged.Broadcast();
		}
	}
}

void AObstacle::Explode()
{
	HideSelf();
	if(ExplosionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAttached(ExplosionParticleSystem, RootSceneComponent);
	}
}
