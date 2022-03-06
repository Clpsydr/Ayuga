#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/UserDefinedStruct.h"
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
	//TMultiMap<int32, FBreadSlice> BreadLibrary;	//difficulty and slices

	//TArray<FBreadSlice> CurrentLibrary;			//temp pylon for searching through certain difficulty

	int Difficulty = 0;

protected:
	virtual void BeginPlay() override;

	void GenerateWave();

	void GenerateSunRay();

	void GenerateWaterDrop();

	void ChangeInterval(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

	void EnableGenerator();

	void DisableGenerator();

	void SpawnBlock(FVector NewPos, int32 BlockIndex);

	void IncreaseDifficulty(int ExtraDiff);
	
	void PurgeEntities();

	UFUNCTION()
		void LaunchMilestoneAnnouncement(int32 NewText);

//	UFUNCTION()
//		void SpawnBrick(TSubclassOf BrickClass, FVector TargetLocation);

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

	// links to various spawned elements
		// assemble to an array
		// get patterns as 2d matrix of int
		// spawn i-th pawn in order
	
	FTimerHandle OneRowTimerHandle;
	FTimerHandle WaterTimerHandle;
	FTimerHandle SunrayTimerHandle;
};
