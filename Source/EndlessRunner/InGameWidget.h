
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class ENDLESSRUNNER_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> P1ScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> P1HighScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> P1SpeedText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> P1HealthProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> P2ScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> P2HighScoreText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> P2SpeedText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> P2HealthProgressBar;
};
