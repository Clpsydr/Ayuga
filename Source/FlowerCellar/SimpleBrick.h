#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleBrick.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class FLOWERCELLAR_API ASimpleBrick : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimpleBrick();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitBox;

	UFUNCTION()
		virtual void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		float FallingSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		int Hardness = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		int Damage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		bool bIsEdible;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		bool bIsHarmful;

	UFUNCTION()
		void Accelerate();

	UFUNCTION()
		void ApplyEffect(AActor* HitActor);

	UFUNCTION()
		void Enable();

	UFUNCTION()
		void Disable();
};
