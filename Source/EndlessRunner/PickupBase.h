
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class UBoxComponent;
class AEndlessRunnerCharacter;

UCLASS()
class ENDLESSRUNNER_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootSceneComponent;

	void HideSelf();
	void ShowSelf();
	bool bIsSelfHidden;

	virtual void ResetSelf();

private:
	UPROPERTY(EditAnywhere)
    TObjectPtr<UBoxComponent> BoxCollider;
    	
    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMeshComponent> StaticMesh;
	
public:	
	APickupBase();

protected:
	UFUNCTION()
	void OnBoxColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
	
	virtual void OnPickup(TObjectPtr<AEndlessRunnerCharacter> Player);
};
