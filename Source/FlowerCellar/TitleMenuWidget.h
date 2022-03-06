#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMenuWidget.generated.h"

class UButton;

/**
 * Main menu functionality at the start of the game
 */
UCLASS()
class FLOWERCELLAR_API UTitleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
		UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
			UButton* NewGameButton;	

		UFUNCTION()
			void OnNewGameClicked();

		UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnimOptional), Transient)
			UWidgetAnimation* NewGameStartAnim;

public:
	virtual void NativeConstruct() override;
};
