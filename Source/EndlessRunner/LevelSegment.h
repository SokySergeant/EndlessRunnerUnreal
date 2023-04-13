
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LevelSegment.generated.h"

UCLASS()
class ENDLESSRUNNER_API ALevelSegment : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollider;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EndLocationComponent;
	
public:	
	ALevelSegment();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FVector GetEndLocation() {return EndLocationComponent->GetComponentLocation();};
};
