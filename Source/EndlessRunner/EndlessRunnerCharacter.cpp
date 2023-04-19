
#include "EndlessRunnerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyGameInstance.h"

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
}

void AEndlessRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;

	//Add Input Mapping Context
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
}

void AEndlessRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::Move);
	}
}

void AEndlessRunnerCharacter::Move(const FInputActionValue& Value)
{
	if(GetWorld()->GetTimerManager().IsTimerActive(MoveToLaneHandle)) return; //if currently busy moving to a lane, return
	
	float MovementVector = Value.Get<float>();
	
	if(MovementVector < 0.f && CurrentLane <= 0) return; //tried moving left while in the leftmost lane
	if(MovementVector > 0.f && CurrentLane >= MyGameInstance->LaneYValues.Num() - 1) return; //tried moving right while in the rightmost lane
	
	CurrentLane += FMath::Sign(MovementVector);

	//get target y value of new lane
	TargetYValue = MyGameInstance->LaneYValues[CurrentLane];
	
	GetWorld()->GetTimerManager().SetTimer(MoveToLaneHandle, this, &AEndlessRunnerCharacter::MoveToLane, GetWorld()->GetDeltaSeconds(), true);
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

void AEndlessRunnerCharacter::UpdateHealthBy(int Hp)
{
	CurrentHp += Hp;

	UE_LOG(LogTemp, Warning, TEXT("HP is at: %d"), CurrentHp);

	if(CurrentHp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Died!"));
		OnPlayerDeath.ExecuteIfBound();
	}
}
