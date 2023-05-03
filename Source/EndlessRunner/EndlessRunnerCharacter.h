
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
class ALevelSegmentsManager;
class UMySaveGame;

UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter
{
	GENERATED_BODY()
	
	FTimerHandle MoveToLaneHandle;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathDelegate);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Player1MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Player1JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Player2MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Player2JumpAction;
	
	UPROPERTY(EditAnywhere)
	float LaneChangeTolerance;
	
	UPROPERTY(EditAnywhere, meta = (UIMin = "0.0", UIMax = "1.0"))
	float LaneChangeSpeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEndlessRunnerCharacter> Player2Blueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameWidget> InGameWidgetBlueprint;
	
	TObjectPtr<UInGameWidget> InGameWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOverWidgetBlueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALevelSegmentsManager> LevelSegmentManagerBlueprint;

	TObjectPtr<AEndlessRunnerCharacter> Player2;

	void SetupLanes();
	void SetupInput();
	void SetupInGameWidget();
	void SetupSecondPlayer();
	void SetupMyLevelSegmentsManager();
	
public:
	AEndlessRunnerCharacter();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Player2Move(const FInputActionValue& Value);
	void Player2Jump(const FInputActionValue& Value);
	void Player2StopJumping(const FInputActionValue& Value);
	
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	
	virtual void BeginPlay();

	int CurrentLane;
	float TargetYValue;
	void MoveToLane();

	float MaxHp = 3;
	float CurrentHp;

	void Die();

	UFUNCTION()
	void EndGameIfBothPlayersDied();
	
	void SaveData();
	void LoadSavedData();
	TObjectPtr<UMySaveGame> SavedData;

	TArray<float> LaneYValues;

public:
	void UpdateHealthBy(float Hp);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathDelegate OnPlayerDeath;

	UPROPERTY(EditAnywhere)
	bool bIsFirstPlayer;

	TObjectPtr<ALevelSegmentsManager> MyLevelSegmentManager;
};

