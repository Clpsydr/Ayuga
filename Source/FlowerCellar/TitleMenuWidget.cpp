#include "TitleMenuWidget.h"
#include "Components/Button.h"
#include "FlowerCellarGameMode.h"
#include "Kismet/GameplayStatics.h"

void UTitleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameButton)
	{
		NewGameButton->OnPressed.AddDynamic(this, &ThisClass::OnNewGameClicked);
	}
}

void UTitleMenuWidget::OnNewGameClicked()
{
	Cast<AFlowerCellarGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StartGame();

}