#include "FlowerCellarGameMode.h"
#include "FlowerPawn.h"
#include "BrickGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AFlowerCellarGameMode::AFlowerCellarGameMode()
{
	DefaultPawnClass = AFlowerPawn::StaticClass();
}

void AFlowerCellarGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RemainingDistance -= DeltaTime;
	AccumulatedDifficulty += DeltaTime;

	if (RemainingDistance <= 0.f)
	{
		StopSystems();
		Victory();
	}
	else
	{
		//visuals for expanding light source?
		VisualShine[0]->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f) * (1 + 9 *(1 - RemainingDistance / TotalDistance )) );
	}

	//if threshold isnt even, make an array of values and counter to the current one
	if (AccumulatedDifficulty > MilestoneThreshold)
	{
		MilestoneReached();
	}

	if (CurrentPlayer)
	{
		if (Cast<AFlowerPawn>(CurrentPlayer)->GetLifeForce() < 0.f || Cast<AFlowerPawn>(CurrentPlayer)->GetPetals() < 1)
		{
			StopSystems();
			GameOver();
		}
	}
}

void AFlowerCellarGameMode::BeginPlay()
{
	Super::BeginPlay();
	CurrentPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerControl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	BrickSource = GetWorld()->SpawnActor<ABrickGenerator>(BrickSourceType, FVector(7500.f, 0.f, 0.f), FRotator(0.f,0.f,0.f));

	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "ShineGoal", VisualShine);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "TubeWalls", LevelWalls);
}

void AFlowerCellarGameMode::StartSystems()
{
	// purge all blocks
	// reset position and parameters of bricksource
	RemainingDistance = TotalDistance;
	AccumulatedDifficulty = 0.f;
	VisualShine[0]->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));

	if (PlayerControl)
	{
		PlayerControl->SetShowMouseCursor(false);
	}

	if (CurrentPlayer)
	{
		CurrentPlayer->SetActorTickEnabled(true);
		Cast<AFlowerPawn>(CurrentPlayer)->ResetParams();
	}

	WallControl(true);

	if (BrickSource)
	{
		BrickSource->PurgeEntities();
		BrickSource->EnableGenerator(InitialVelocity);
	}
}

void AFlowerCellarGameMode::StopSystems()
{
	if (PlayerControl)
	{
		PlayerControl->SetShowMouseCursor(true);
	}

	if (CurrentPlayer)
	{
		CurrentPlayer->SetActorTickEnabled(false);
	}

	WallControl(false);

	if (BrickSource)
	{
		BrickSource->DisableGenerator();
	}
}

FString AFlowerCellarGameMode::GetPlayerHP()
{
	if (Cast<AFlowerPawn>(CurrentPlayer))
	{
		return FString::FromInt(Cast<AFlowerPawn>(CurrentPlayer)->GetPetals());
	}
	return FString("nothing");
}

FString AFlowerCellarGameMode::GetPlayerEnergy()
{
	if (Cast<AFlowerPawn>(CurrentPlayer))
	{
		return FString::FromInt(Cast<AFlowerPawn>(CurrentPlayer)->GetLifeForce());
	}
	return FString("nothing");
}

float AFlowerCellarGameMode::GetPlayerResourceRatio()
{
	if (Cast<AFlowerPawn>(CurrentPlayer))
	{
		return Cast<AFlowerPawn>(CurrentPlayer)->GetResourceRatio();
	}
	return 0.f;
}

void AFlowerCellarGameMode::WallControl(bool ToStartMoving)
{
	if (LevelWalls.Num() > 0)
	{
		for (AActor* Wall : LevelWalls)
		{
			if (ToStartMoving)
			{
				Wall->SetActorTickEnabled(true);
			}
			else
			{
				Wall->SetActorTickEnabled(false);
			}

		}
	}
}

void AFlowerCellarGameMode::SwitchUIControl(bool bEnableUIControl)
{
	if (PlayerControl)
	{
		PlayerControl->SetShowMouseCursor(bEnableUIControl);
		if (bEnableUIControl)
		{
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerControl);
		}
		else
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerControl);
		}
	}
}

void AFlowerCellarGameMode::MilestoneReached()
{
	AccumulatedDifficulty = 0.f;
	if (BrickSource)
	{
		BrickSource->IncreaseDifficulty(1);
	}
	GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("Difficulty increases")));
}