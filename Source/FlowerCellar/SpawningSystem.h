#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawningSystem.generated.h"

/**
 * System responsible for issuing and deactivating expendable objects
 */
UCLASS()
class FLOWERCELLAR_API USpawningSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	AActor* RetrieveActor(UClass* Class, const FTransform& Transform);
	void ReturnActor(AActor* Actor);
	void PurgeActiveActors();
	bool IsActorInPool(AActor* Actor) const;

protected:
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

private:
	UFUNCTION()
		void OnActorDestroyed(AActor* Actor);

	UPROPERTY()
		TArray<class AActor*> ActorArray;
		TMap<UClass*, TArray<class AActor*>> InactiveActors;
};
