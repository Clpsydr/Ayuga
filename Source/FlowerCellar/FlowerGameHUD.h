#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FlowerGameHUD.generated.h"

class UUserWidget;

UENUM(BlueprintType)
enum class EWidgetEnum : uint8
{
	Wid_None,
	Wid_StartGame,
	Wid_Gameplay,
	Wid_Gameover,
	Wid_Victory,
};

/**
 * Hud to manage start screen, gameover and progress
 */
UCLASS()
class FLOWERCELLAR_API AFlowerGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFlowerGameHUD();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		UUserWidget* ShowWidget(const EWidgetEnum EWidgetId, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
		void HideWidget();

	UFUNCTION(BlueprintPure)
		UUserWidget* GetCurrentWidget() const;

protected:
	UFUNCTION()
		UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);

	UPROPERTY(EditAnywhere)
		TMap<EWidgetEnum, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
		UUserWidget* CurrentWidget;
};
