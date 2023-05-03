
#include "MainMenuWidget.h"
#include "MySaveGame.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SavedData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("FirstSaveSlot"), 0));

	if(SavedData)
	{
		P1HighScoreText->SetText(FText::AsNumber(FMath::TruncToInt(SavedData->P1HighScore)));
		P2HighScoreText->SetText(FText::AsNumber(FMath::TruncToInt(SavedData->P2HighScore)));
	}
}
