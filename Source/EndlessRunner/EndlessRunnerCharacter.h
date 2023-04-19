
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EndlessRunnerCharacter.generated.h"

struct FInputActionValue;
class UMyGameInstance;
class UInputAction;
class UInputMappingContext;

UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	//MoveTimer
	FTimerHandle MoveToLaneHandle;

	DECLARE_DELEGATE(OnPlayerDeathDelegate);
	
	//InputActions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	//Other values
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

	TObjectPtr<UMyGameInstance> MyGameInstance;

	int CurrentLane;
	float TargetYValue;
	void MoveToLane();

	int MaxHp = 3;
	int CurrentHp;

public:
	void UpdateHealthBy(int Hp);

	OnPlayerDeathDelegate OnPlayerDeath;
};

