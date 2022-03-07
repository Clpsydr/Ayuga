#pragma once
#include "CoreMinimal.h"
#include "SimpleBrick.h"
#include "FlyingBrick.generated.h"

/**
 * Brick with specific moving rules and collision
 */
UCLASS()
class FLOWERCELLAR_API AFlyingBrick : public ASimpleBrick
{
	GENERATED_BODY()

public:
	AFlyingBrick();

private:
	UPROPERTY()
		FVector FlyDirection;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, 
					AActor* OtherActor, 
					UPrimitiveComponent* OtherComp, 
					FVector NormalImpulse, 
					const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params");
		float FlyVelocty = 1000.f;

};
