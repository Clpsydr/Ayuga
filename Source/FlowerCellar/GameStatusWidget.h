#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatusWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * Visuals on game progress
 */
UCLASS()
class FLOWERCELLAR_API UGameStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* ResourceBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* RemainingHP;
		 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
		UTextBlock* RemainingTime;

	//add a flipbook with several tutorial pages 
	//add an array of tutorial page uisection

	UFUNCTION(BlueprintCallable)
	void UpdateProgress(float NewFraction);
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(FString NewParam);

	UFUNCTION(BlueprintCallable)
	void UpdateResource(float NewFraction);

	UFUNCTION(BlueprintCallable)
	void UpdateEnergy(FString NewParam);

};
