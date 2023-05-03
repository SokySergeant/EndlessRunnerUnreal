
#include "Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bCanTriggerExplosionInOtherObstacle = false;
	ProjectileSpeed = 10.f;

	RelativeLocationOffset = FVector(0.f, 0.f, 0.f);
}

void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RelativeLocationOffset += DeltaSeconds * FVector(-ProjectileSpeed, 0.f, 0.f);
	SetActorRelativeLocation(RelativeLocationOffset);
}

void AProjectile::ResetSelf()
{
	Super::ResetSelf();
	
	RelativeLocationOffset = FVector(0.f, 300.f * FMath::RandRange(-1, 1), 0.f);
}
