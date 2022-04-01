#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/UserDefinedStruct.h"
#include "ObstaclePattern.h"
#include "BrickGenerator.generated.h"

class ASunray;
class ASimpleBrick;

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	None,
	NormalBlock,
	SoftBlock,
	FlyBlock,
	WaterDrop,
	Sunray,
};

USTRUCT(BlueprintType)
struct FBreadSlice
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<EBlockType> BlockSet;
	UPROPERTY()
		int32 RepeatNumber {1};

	UPROPERTY()
		int32 MinimalDifficulty {0};		//replace with probability?

	FBreadSlice() {}
	FBreadSlice(TArray<EBlockType> NewSet) : BlockSet(NewSet) 
	{}
	FBreadSlice(TArray<EBlockType> NewSet, int32 NewNumber, int32 NewDiff) :		
		BlockSet(NewSet), RepeatNumber(NewNumber), MinimalDifficulty(NewDiff)
	{}
	//repeatnumber of -1 would not exhaust the slice from currentlibrary
};

UCLASS()
class FLOWERCELLAR_API ABrickGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ABrickGenerator();

private:	
	TMultiMap<int32, FBrickPattern*> PatternLib;	//difficulty and slices

	TArray<FBrickPattern*> CurrentDiffLib;		//temp pylon for searching through certain difficulty

	int Difficulty = 0;

	float InitialBrickVelocity = 0.f;

	UPROPERTY()
	TArray<FVector2D> PossiblePoints;				//relative positions of all spawnpoints

protected:
	virtual void BeginPlay() override;

	void GenerateWave();

	void GenerateSunRay();

	void GenerateWaterDrop();

	void ChangeInterval(float DeltaTime);

	// datatable plug
	UPROPERTY(EditAnywhere)
		UDataTable* BrickPatternSource;

public:	
	virtual void Tick(float DeltaTime) override;

	void EnableGenerator(float InitialVelocity);

	void DisableGenerator();

	void SpawnBlock(FVector NewPos, int32 BlockIndex);

	void IncreaseDifficulty(int ExtraDiff);

	void CollectPatterns();

	void AttachNewDifficultySet(int NewDifficulty);
	
	void PurgeEntities();

	UFUNCTION()
		void LaunchMilestoneAnnouncement(int32 NewText);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Elements")
		TArray<TSubclassOf<ASimpleBrick>> BrickSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Elements")
		TSubclassOf<ASimpleBrick> WaterBonusClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Elements")
		TSubclassOf<ASunray> SunrayBonusClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Elements")
		TSubclassOf<ASimpleBrick> AnnouncementClass;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Elements")
	//TArray<TSubclassOf<class BrickScene>> BrickPatterns;
		//would have difficulty tied in 
		//on begin, finds subobjects, and if their coords are closer to center, speeds them up or mutates
		//depending on diff

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn properties")
		int PatternSize = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn properties")
		float WaveInterval = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn properties")
		float WaterFirstInterval = 3.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn properties")
		float SunrayFirstInterval = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn properties")
		float WaterInterval = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn properties")
		float SunrayInterval = 3.f;

	

	FTimerHandle OneRowTimerHandle;
	FTimerHandle WaterTimerHandle;
	FTimerHandle SunrayTimerHandle;
};
