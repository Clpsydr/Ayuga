#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "ObstaclePattern.generated.h"

UENUM()
enum class EBrickTypes : uint8
{
	Br_None,
	Br_Soft,
	Br_Hard,
	Br_Water,
	Br_Fly
};

USTRUCT(BlueprintType)
struct FBrickPattern : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Main")
		FName Id;

	UPROPERTY(EditDefaultsOnly, Category = "Main")
		int Difficulty;

	UPROPERTY(EditDefaultsOnly, Category = "Main")
		FString Pattern;

	UPROPERTY(EditDefaultsOnly, Category = "Extras")
		float DelayToNext;

	UPROPERTY(EditDefaultsOnly, Category = "Extras")
		float Extras;
};