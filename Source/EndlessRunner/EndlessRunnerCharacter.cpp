
#include "EndlessRunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AEndlessRunnerCharacter::AEndlessRunnerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true; 

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CurrentLane = 2;
	LaneChangeTolerance = 10.f;
	LaneChangeSpeed = 0.8f;
}

void AEndlessRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

	UE_LOG(LogTemp, Warning, TEXT("%f"), MovementVector);
	
	if(MovementVector < 0.f && CurrentLane <= 1) return; //tried moving left while in the leftmost lane
	if(MovementVector > 0.f && CurrentLane >= 3) return; //tried moving right while in the rightmost lane
	
	CurrentLane += FMath::Sign(MovementVector);

	//get target y value of new lane
	switch (CurrentLane)
	{
	case 1:
		TargetYValue = MyGameInstance->Lane1YValue;
		break;
	case 2:
		TargetYValue = MyGameInstance->Lane2YValue;
		break;
	case 3:
		TargetYValue = MyGameInstance->Lane3YValue;
		break;
	default:
		TargetYValue = MyGameInstance->Lane2YValue;
		break;
	}
	
	GetWorld()->GetTimerManager().SetTimer(MoveToLaneHandle, this, &AEndlessRunnerCharacter::MoveToLane, GetWorld()->GetDeltaSeconds(), true);
}

void AEndlessRunnerCharacter::MoveToLane()
{
	FVector newLoc = FVector(GetActorLocation().X, FMath::Lerp(GetActorLocation().Y, TargetYValue, LaneChangeSpeed), GetActorLocation().Z);
	SetActorLocation(newLoc);

	float distanceFromTarget = TargetYValue - GetActorLocation().Y;
	if(FMath::Abs(distanceFromTarget) <= LaneChangeTolerance)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveToLaneHandle);
	}
}
