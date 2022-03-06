#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"

class UButton;

/**
 * Actually a game over widget. Yes, we are this much close to deadline.
 */
UCLASS()
class FLOWERCELLAR_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
		UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
			UButton* RestartButton;

		UFUNCTION()
			void OnRestartClicked();

public:
	virtual void NativeConstruct() override;
	
};
