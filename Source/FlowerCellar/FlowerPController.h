#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FlowerPController.generated.h"

class AFlowerPawn;

/**
 * basic player controller - flower movement and extra actions
 */
UCLASS()
class FLOWERCELLAR_API AFlowerPController : public APlayerController
{
	GENERATED_BODY()
	
	protected:
		UPROPERTY()
			AFlowerPawn* FlowerPawn;

public:
	AFlowerPController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	void VAxisMovement(float AxisValue);
	void HAxisMovement(float AxisValue);
	void UseItem();
};
