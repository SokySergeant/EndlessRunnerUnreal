
#include "EndlessRunnerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InGameWidget.h"
#include "LevelSegmentsManager.h"
#include "MySaveGame.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameOverWidget.h"
#include "Components/TextBlock.h"

AEndlessRunnerCharacter::AEndlessRunnerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CurrentLane = 1;
	LaneChangeTolerance = 10.f;
	LaneChangeSpeed = 0.8f;
	
	bIsFirstPlayer = false;
}

void AEndlessRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadSavedData();

	CurrentHp = MaxHp;
	
	SetupLanes();

	if(!bIsFirstPlayer) return;
	
	SetupInGameWidget();
	SetupMyLevelSegmentsManager();
	SetupSecondPlayer();
	SetupInput();

	OnPlayerDeath.AddDynamic(this, &AEndlessRunnerCharacter::EndGameIfBothPlayersDied);
}

void AEndlessRunnerCharacter::SetupLanes()
{
	LaneYValues.Add(GetActorLocation().Y - 300.f);
	LaneYValues.Add(GetActorLocation().Y);
	LaneYValues.Add(GetActorLocation().Y + 300.f);
}

void AEndlessRunnerCharacter::SetupInGameWidget()
{
	InGameWidget = Cast<UInGameWidget>(CreateWidget(GetWorld(), InGameWidgetBlueprint));
	InGameWidget->AddToViewport();
}


void AEndlessRunnerCharacter::SetupMyLevelSegmentsManager()
{
	FActorSpawnParameters SpawnParams;
	MyLevelSegmentManager = GetWorld()->SpawnActor<ALevelSegmentsManager>(LevelSegmentManagerBlueprint, GetActorLocation() + FVector(-3500.f, 0.f, -240.f), GetActorRotation(), SpawnParams);
	MyLevelSegmentManager->MyPlayer = this;
	MyLevelSegmentManager->InGameWidget = InGameWidget;
	
	if(SavedData)
	{
		if(bIsFirstPlayer)
		{
			MyLevelSegmentManager->HighScore = SavedData->P1HighScore;
		}else
		{
			MyLevelSegmentManager->HighScore = SavedData->P2HighScore;
		}
	}
	
	MyLevelSegmentManager->StartSetup();
}

void AEndlessRunnerCharacter::SetupSecondPlayer()
{
	FActorSpawnParameters SpawnParams;
	Player2 = GetWorld()->SpawnActor<AEndlessRunnerCharacter>(Player2Blueprint, GetActorLocation() + FVector(0.f, 1000.f, 0.f), GetActorRotation(), SpawnParams);
	Player2->InGameWidget = InGameWidget;
	Player2->SetupMyLevelSegmentsManager();
	Player2->OnPlayerDeath.AddDynamic(this, &AEndlessRunnerCharacter::EndGameIfBothPlayersDied);
}

void AEndlessRunnerCharacter::SetupInput()
{
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEndlessRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(Player1JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(Player1JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(Player1MoveAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::Move);

		EnhancedInputComponent->BindAction(Player2JumpAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::Player2Jump);
		EnhancedInputComponent->BindAction(Player2JumpAction, ETriggerEvent::Completed, this, &AEndlessRunnerCharacter::Player2StopJumping);
		EnhancedInputComponent->BindAction(Player2MoveAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::Player2Move);
	}
}

void AEndlessRunnerCharacter::Move(const FInputActionValue& Value)
{
	if(GetWorld()->GetTimerManager().IsTimerActive(MoveToLaneHandle)) return; //if currently busy moving to a lane, return
	
	float MovementVector = Value.Get<float>();
	
	if(MovementVector < 0.f && CurrentLane <= 0) return; //tried moving left while in the leftmost lane
	if(MovementVector > 0.f && CurrentLane >= LaneYValues.Num() - 1) return; //tried moving right while in the rightmost lane
	
	CurrentLane += FMath::Sign(MovementVector);

	//get target y value of new lane
	TargetYValue = LaneYValues[CurrentLane];
	
	GetWorld()->GetTimerManager().SetTimer(MoveToLaneHandle, this, &AEndlessRunnerCharacter::MoveToLane, GetWorld()->GetDeltaSeconds(), true);
}

void AEndlessRunnerCharacter::Player2Move(const FInputActionValue& Value)
{
	if(Player2)
	{
		Player2->Move(Value);
	}
}

void AEndlessRunnerCharacter::Player2Jump(const FInputActionValue& Value)
{
	if(Player2)
	{
		Player2->Jump();
	}
}

void AEndlessRunnerCharacter::Player2StopJumping(const FInputActionValue& Value)
{
	if(Player2)
	{
		Player2->StopJumping();
	}
}

void AEndlessRunnerCharacter::MoveToLane()
{
	FVector NewLoc = FVector(GetActorLocation().X, FMath::Lerp(GetActorLocation().Y, TargetYValue, LaneChangeSpeed), GetActorLocation().Z);
	SetActorLocation(NewLoc);

	float DistanceFromTarget = TargetYValue - GetActorLocation().Y;
	if(FMath::Abs(DistanceFromTarget) <= LaneChangeTolerance)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveToLaneHandle);
	}
}

void AEndlessRunnerCharacter::UpdateHealthBy(float Hp)
{
	CurrentHp += Hp;
	CurrentHp = FMath::Clamp(CurrentHp, 0, MaxHp);
	
	if(CurrentHp == 0)
	{
		Die();
	}

	if(!InGameWidget) return;
	if(bIsFirstPlayer)
	{
		InGameWidget->P1HealthProgressBar->SetPercent(CurrentHp / MaxHp);
	}else
	{
		InGameWidget->P2HealthProgressBar->SetPercent(CurrentHp / MaxHp);
	}
}

void AEndlessRunnerCharacter::Die()
{
	SetActorHiddenInGame(true);
	
	if(OnPlayerDeath.IsBound())
	{
		OnPlayerDeath.Broadcast();
	}
}

void AEndlessRunnerCharacter::EndGameIfBothPlayersDied()
{
	if(!IsHidden() || !Player2->IsHidden()) return; //if one of the players is still alive, return
	
	EnhancedInputComponent->ClearActionBindings();
	
	SaveData();

	//show and setup game over widget
	TObjectPtr<UGameOverWidget> GameOverWidget = Cast<UGameOverWidget>(CreateWidget(GetWorld(), GameOverWidgetBlueprint));
	GameOverWidget->AddToViewport();

	if(MyLevelSegmentManager->Score > Player2->MyLevelSegmentManager->Score)
	{
		GameOverWidget->GameOverInfoText->SetText(FText::FromString("Player 1 won!"));
	}else
	{
		GameOverWidget->GameOverInfoText->SetText(FText::FromString("Player 2 won!"));
	}
	
	Cast<APlayerController>(GetController())->bShowMouseCursor = true;
}

void AEndlessRunnerCharacter::SaveData()
{
	TObjectPtr<UMySaveGame> SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	
	SaveGameInstance->P1HighScore = MyLevelSegmentManager->HighScore;
	SaveGameInstance->P2HighScore = Player2->MyLevelSegmentManager->HighScore;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("FirstSaveSlot"), 0);
}

void AEndlessRunnerCharacter::LoadSavedData()
{
	SavedData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("FirstSaveSlot"), 0));
}
