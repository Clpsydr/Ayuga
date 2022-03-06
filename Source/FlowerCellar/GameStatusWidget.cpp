#include "GameStatusWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UGameStatusWidget::UpdateProgress(float NewFraction)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(NewFraction);
	}
}

void UGameStatusWidget::UpdateHealth(FString NewParam)
{
	if (RemainingTime)
	{
		FText NewText;
		RemainingTime->SetText(NewText.FromString(NewParam));
	}
}

void UGameStatusWidget::UpdateEnergy(FString NewParam)
{
	if (RemainingHP)
	{
		FText NewText;
		RemainingHP->SetText(NewText.FromString(NewParam));
	}
}

void UGameStatusWidget::UpdateResource(float NewFraction)
{
	if (ResourceBar)
	{
		ResourceBar->SetFillColorAndOpacity(FLinearColor(0.f, 0.f, 0.5f, 1.f));

		if (NewFraction > 0.5f)
		{
			ResourceBar->SetFillColorAndOpacity(FLinearColor(0.f, 0.f, 1.f, 1.f));

		}
		else if (NewFraction == 1.f)
		{
			ResourceBar->SetFillColorAndOpacity(FLinearColor(0.f, 1.f, 1.f, 1.f));
		}

		ResourceBar->SetPercent(NewFraction);
	}
}