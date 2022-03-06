#include "EndGameWidget.h"
#include "Components/Button.h"
#include "FlowerCellarGameMode.h"
#include "Kismet/GameplayStatics.h"

void UEndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartButton)
	{
		RestartButton->OnPressed.AddDynamic(this, &ThisClass::OnRestartClicked);
	}
}

void UEndGameWidget::OnRestartClicked()
{
	Cast<AFlowerCellarGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StartGame();
}