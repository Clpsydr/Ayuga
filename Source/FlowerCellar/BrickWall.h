/*
* Planned to be the spawned element, that manages bricks within itself
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickWall.generated.h"

UCLASS()
class FLOWERCELLAR_API ABrickWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrickWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
