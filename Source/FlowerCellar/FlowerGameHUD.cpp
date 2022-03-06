#include "FlowerGameHUD.h"
#include "Blueprint/UserWidget.h"

AFlowerGameHUD::AFlowerGameHUD()
{
}

void AFlowerGameHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AFlowerGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UUserWidget* AFlowerGameHUD::ShowWidget(const EWidgetEnum EWidgetId, const int32 ZOrder)
{
	HideWidget();
	TSubclassOf<UUserWidget>* ClassPtr = WidgetClasses.Find(EWidgetId);
	if (ClassPtr && *ClassPtr)
	{
		CreateWidgetByClass(*ClassPtr, ZOrder);
	}

	return CurrentWidget;
}

void AFlowerGameHUD::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* AFlowerGameHUD::GetCurrentWidget() const
{
	return CurrentWidget;
}

UUserWidget* AFlowerGameHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
{
	CurrentWidget = CreateWidget(GetWorld(), WidgetClass);
	CurrentWidget->AddToViewport(ZOrder);
	return CurrentWidget;
}
