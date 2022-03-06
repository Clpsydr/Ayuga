#include "FlowerPController.h"
#include "FlowerPawn.h"
#include "FlowerCellarGameMode.h"
#include "Kismet/GameplayStatics.h"

AFlowerPController::AFlowerPController()
{
}

void AFlowerPController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveUp", this, &ThisClass::VAxisMovement);
	InputComponent->BindAxis("MoveSide", this, &ThisClass::HAxisMovement);
	InputComponent->BindAction("UseItemAction", IE_Pressed, this, &ThisClass::UseItem);
}

void AFlowerPController::BeginPlay()
{
	Super::BeginPlay();

	FlowerPawn = Cast<AFlowerPawn>(GetPawn());

	//bShowMouseCursor = true;
}

void AFlowerPController::Tick(float DeltaSeconds)
{

}

void AFlowerPController::VAxisMovement(float AxisValue)
{
	if (FlowerPawn)
	{
		FlowerPawn->VMovement(AxisValue);
	}
}

void AFlowerPController::HAxisMovement(float AxisValue)
{
	if (FlowerPawn)
	{
		FlowerPawn->HMovement(AxisValue);
	}
}

void AFlowerPController::UseItem()
{
	if (FlowerPawn)
	{
		FlowerPawn->UseItem();
	}
}


