
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EndlessRunnerCharacter.generated.h"

struct FInputActionValue;
class UMyGameInstance;
class UInputAction;
class UInputMappingContext;
class UInGameWidget;
class UGameOverWidget;

UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter
{
	GENERATED_BODY()
	
	FTimerHandle MoveToLaneHandle;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathDelegate);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere)
	float LaneChangeTolerance;
	
	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", UIMax = "1.0"))
	float LaneChangeSpeed;

public:
	AEndlessRunnerCharacter();

protected:
	void Move(const FInputActionValue& Value);
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay();

	int CurrentLane;
	float TargetYValue;
	void MoveToLane();

	float MaxHp = 3;
	float CurrentHp;

	TArray<float> LaneYValues;

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInGameWidget> InGameWidget;
	
	void UpdateHealthBy(float Hp);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathDelegate OnPlayerDeath;
};

