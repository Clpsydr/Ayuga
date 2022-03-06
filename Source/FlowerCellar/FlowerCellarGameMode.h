#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlowerCellarGameMode.generated.h"

class ABrickGenerator;

UCLASS(minimalapi)
class AFlowerCellarGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	int Difficulty = 0;		// increases from remaining distance; possibly float directly affecting rates?

	float AccumulatedDifficulty = 0.f;

	float RemainingDistance = 0.f;

	ABrickGenerator* BrickSource;

	APawn* CurrentPlayer;

	APlayerController* PlayerControl;

	TArray<AActor*> VisualShine; 

	TArray<AActor*> LevelWalls;

	void SwitchUIControl(bool bEnableUIControl);

	void MilestoneReached();

public:
	AFlowerCellarGameMode();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameFlow")
		void StartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameFlow")
		void GameOver();	//state change, UI control

	UFUNCTION(BlueprintImplementableEvent, Category = "GameFlow")
		void Victory();		//state change, UI control

	// difficulty reset, distance and playerstats
	
	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		void StartSystems();	 

	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		void StopSystems();

	//control over walls, start dynamic texture
	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		void WallControl(bool ToStartMoving);

	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		FString GetPlayerHP();

	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		float GetPlayerResourceRatio();

	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		FString GetPlayerEnergy();

	UFUNCTION(BlueprintCallable, Category = "ExtraFunctions")
		float GetCurrentDistance() { return RemainingDistance; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Elements")
		TSubclassOf<class ABrickGenerator> BrickSourceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Balance")
		float TotalDistance = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Balance")
		float MilestoneThreshold = 10.f;

protected:
	virtual void BeginPlay() override;
};