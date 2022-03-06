#include "BrickGenerator.h"
#include "SimpleBrick.h"
#include "Sunray.h"
#include "SpawningSystem.h"

ABrickGenerator::ABrickGenerator()
{
	#define none EBlockType::None
	#define blck EBlockType::NormalBlock
	#define soft EBlockType::SoftBlock
	#define watr EBlockType::WaterDrop
	#define flyy EBlockType::FlyBlock
	#define rayl EBlockType::Sunray

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.TickInterval = 0.005f;

	// invalidted TMap with difficulty as key
	/*BreadLibrary.Add(0, FBreadSlice({
	none,none,none,none,none,
	none,none,none,none,none,
	none,none,none,none,none,
	none,none,none,none,none,
	none,none,none,none,none,
		}, -1, 0));ated until further development
	//undocumen
	*/
}

void ABrickGenerator::BeginPlay()
{
	Super::BeginPlay();
	GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("BrickSet contains %d elements"), BrickSet.Num()));
}

void ABrickGenerator::EnableGenerator()
{
	//initial difficulty shift for accumulating templates
	IncreaseDifficulty(0);
	SetActorTickEnabled(true);
	GetWorld()->GetTimerManager().SetTimer(OneRowTimerHandle, this, &ABrickGenerator::GenerateWave, WaveInterval, true);
	GetWorld()->GetTimerManager().SetTimer(WaterTimerHandle, this, &ABrickGenerator::GenerateWaterDrop, WaterFirstInterval, false);
	GetWorld()->GetTimerManager().SetTimer(SunrayTimerHandle, this, &ABrickGenerator::GenerateSunRay, SunrayFirstInterval, false);
}

void ABrickGenerator::DisableGenerator()
{
	SetActorTickEnabled(false);
	Difficulty = 0;
	GetWorld()->GetTimerManager().ClearTimer(OneRowTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(WaterTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SunrayTimerHandle);
	//CurrentLibrary.Empty();
}

void ABrickGenerator::GenerateWave()
{
	//get from roster random line, if its 1, remove it from library
	//iterate through list of enums to spawn corresponding item in corresponding place.
	//FBreadSlice ChosenSlice = CurrentLibrary[FMath::RandRange(0, CurrentLibrary.Num()-1)];
	
	//Selecting unique spawn points, more with higher difficulty
	int TargetAmountOfSpawns = 8 + Difficulty;
	TArray<FVector2D> SpawnPoints;
	//dont do this at home, kids
	for (int i = 0; i < TargetAmountOfSpawns; i++)
	{
		float a = FMath::RandRange(0, PatternSize-1);
		float b = FMath::RandRange(0, PatternSize-1);
		FVector2D NewVector = FVector2D(a,b);
		if (SpawnPoints.Find(NewVector) == INDEX_NONE)
		{
			SpawnPoints.Add(NewVector);
		}
		else
		{
			i--;
		}
	}

	//copy array of all points, exempt any few random points out of it instead??
	/*TArray<FVector2D> PossiblePoints;

	for (int i = 0; i < TargetAmountOfSpawns; i++)
	{
		FVector2D NewPoints = TargetAmountOfSpawns
		SpawnPoints.Add(TargetAmountOfSpawns)
		// first populate the vector , then pull it out
		// or event better populate it with just 0 1 2 3 4 so on, thats enough
	}*/



	for (FVector2D SpawnPoint : SpawnPoints)
	{
		SpawnBlock(FVector(GetActorLocation().X, 
							GetActorLocation().Y - 300 * PatternSize + 600 * SpawnPoint.X + 250, 
							GetActorLocation().Z - 300 * PatternSize + 600 * SpawnPoint.Y + 250), 
					FMath::FRandRange(0, 45) / 15);		// make proper division??
		
	}
}

// sunray specifics are spawning location
void ABrickGenerator::GenerateSunRay()
{
	if (SunrayBonusClass)
	{
		USpawningSystem* Pool = GetWorld()->GetSubsystem<USpawningSystem>();
		
		FTransform SpawnTransform;

		int32 RandomDecision = FMath::RandRange(1, 4);

		switch (RandomDecision)
		{
			case 1:
				SpawnTransform = FTransform(GetActorRotation(), FVector(GetActorLocation().X,
					GetActorLocation().Y - 1350.f ,
					GetActorLocation().Z + FMath::FRandRange(-700.f, 700.f)),
					FVector::OneVector);
				break;
			case 2:
				SpawnTransform = FTransform(GetActorRotation(), FVector(GetActorLocation().X,
					GetActorLocation().Y + 1350.f,
					GetActorLocation().Z + FMath::FRandRange(-700.f, 700.f)),
					FVector::OneVector);
				break;
			case 3:
				SpawnTransform = FTransform(GetActorRotation(), FVector(GetActorLocation().X,
					GetActorLocation().Y + FMath::FRandRange(-700.f, 700.f),
					GetActorLocation().Z - 1350.f),
					FVector::OneVector);
				break;
			case 4:
				SpawnTransform = FTransform(GetActorRotation(), FVector(GetActorLocation().X,
					GetActorLocation().Y + FMath::FRandRange(-700.f, 700.f),
					GetActorLocation().Z + 1350.f),
					FVector::OneVector);
				break;
			default:
				SpawnTransform = FTransform(GetActorRotation(), FVector(GetActorLocation().X,
					GetActorLocation().Y,
					GetActorLocation().Z),
					FVector::OneVector);
		}

		auto* NewBonus = Cast<ASimpleBrick>(Pool->RetrieveActor(SunrayBonusClass, SpawnTransform));
		//auto* NewBonus = GetWorld()->SpawnActor<ASunray>(SunrayBonusClass, GetActorLocation(), FRotator(0.f, 0.f, 0.f));
		
		if (NewBonus)
		{
			GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("sunray is spawned")));
			NewBonus->Enable();
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(SunrayTimerHandle, this, &ABrickGenerator::GenerateSunRay, SunrayInterval, true);
}

//waterdrop specifics are spawning location
void ABrickGenerator::GenerateWaterDrop()
{
	if (WaterBonusClass)
	{
		USpawningSystem* Pool = GetWorld()->GetSubsystem<USpawningSystem>();
		FTransform SpawnTransform(GetActorRotation(), FVector(GetActorLocation().X,
			GetActorLocation().Y + FMath::FRandRange(-1000.f, 1000.f),
			GetActorLocation().Z + FMath::FRandRange(-1000.f, 1000.f)),
			FVector::OneVector);

		auto* NewBonus = Cast<ASimpleBrick>(Pool->RetrieveActor(WaterBonusClass, SpawnTransform));

		if (NewBonus)
		{
			GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("droplet is spawned")));
			Cast<ASimpleBrick>(NewBonus)->Enable();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(WaterTimerHandle, this, &ABrickGenerator::GenerateWaterDrop, WaterInterval, true);
}

void ABrickGenerator::SpawnBlock(FVector NewPos, int32 BlockIndex)
{
	if (BrickSet[BlockIndex])
	{
		USpawningSystem* Pool = GetWorld()->GetSubsystem<USpawningSystem>();
		FTransform SpawnTransform(GetActorRotation(), NewPos, FVector::OneVector);
		ASimpleBrick* NewBrick = Cast<ASimpleBrick>(Pool->RetrieveActor(BrickSet[BlockIndex], SpawnTransform));

		if (NewBrick)
		{
			NewBrick->Enable();

			//acceleration process
			if (NewBrick->GetActorLocation().Y < 1100.f && NewBrick->GetActorLocation().Z < 1100.f &&
				NewBrick->GetActorLocation().Y > 300.f && NewBrick->GetActorLocation().Z > 300.f)
			{
				if (FMath::RandRange(0.f, 15.f) < 9.f)
				{
					NewBrick->Accelerate();
				}
			}
		}
	}
}

void ABrickGenerator::IncreaseDifficulty(int ExtraDiff)
{
	Difficulty += ExtraDiff;
	LaunchMilestoneAnnouncement(Difficulty);
	//for additive difficulty move all new difficulties to current library, if difficulty <= current
}

void ABrickGenerator::PurgeEntities()
{
	/*USpawningSystem* Pool = GetWorld()->GetSubsystem<USpawningSystem>();
	Pool->PurgeActiveActors();*/
}

//spawn class that flies along with a milestone announcement widget
void ABrickGenerator::LaunchMilestoneAnnouncement(int32 NewText)
{
	USpawningSystem* Pool = GetWorld()->GetSubsystem<USpawningSystem>();
	FTransform SpawnTransform(GetActorRotation(), GetActorLocation(), FVector::OneVector);
	ASimpleBrick* NewBrick = Cast<ASimpleBrick>(Pool->RetrieveActor(AnnouncementClass, SpawnTransform));

	if (NewBrick)
	{
		GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("announcement a go!")));
		Cast<ASimpleBrick>(NewBrick)->Enable();
	}

	// garbage dump
	//NewPetal = GetWorld()->SpawnActor<AActor>(PetalType,	GetActorLocation(),	FRotator(0.f, 0.f, (Petals / 180.f) * i));
	//ASimpleBrick* NewBonus = GetWorld()->SpawnActor<ASimpleBrick>(WaterBonusClass, SpawnTransform, FRotator(0.f,0.f,0.f));
	//auto* NewBonus = GetWorld()->SpawnActor<AActor>(WaterBonusClass, GetActorLocation() + SpawnTransform, FRotator(0.f, 0.f, 0.f));
}

void ABrickGenerator::ChangeInterval(float DeltaTime)
{
}

void ABrickGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

