#include "BrickGenerator.h"
#include "SimpleBrick.h"
#include "FlyingBrick.h"
#include "Sunray.h"
#include "SpawningSystem.h"

ABrickGenerator::ABrickGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.TickInterval = 0.005f;
}

void ABrickGenerator::BeginPlay()
{
	Super::BeginPlay();
	//populating pool of possible to use points
	for (int i = 0; i < PatternSize * PatternSize; i++)
	{
		PossiblePoints.Add(FVector2D(i / PatternSize, i % PatternSize));
		GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("%d %d"), i / PatternSize, i % PatternSize));
	}

	CollectPatterns();
}

void ABrickGenerator::EnableGenerator(float InitialVelocity)
{
	//initial difficulty shift for accumulating templates
	IncreaseDifficulty(0);
	SetActorTickEnabled(true);

	GetWorld()->GetTimerManager().SetTimer(OneRowTimerHandle, this, &ABrickGenerator::GenerateWave, WaveInterval, true);
	GetWorld()->GetTimerManager().SetTimer(WaterTimerHandle, this, &ABrickGenerator::GenerateWaterDrop, WaterFirstInterval, false);
	GetWorld()->GetTimerManager().SetTimer(SunrayTimerHandle, this, &ABrickGenerator::GenerateSunRay, SunrayFirstInterval, false);

	InitialBrickVelocity = InitialVelocity;
}

void ABrickGenerator::DisableGenerator()
{
	SetActorTickEnabled(false);
	Difficulty = 0;
	CurrentDiffLib.Empty();

	GetWorld()->GetTimerManager().ClearTimer(OneRowTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(WaterTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SunrayTimerHandle);
}


// Wave is generated from existing datatable, if it was parsed properly.
// Otherwise wave is constructed randomly
void ABrickGenerator::GenerateWave()
{
	if (CurrentDiffLib.Num() > 0)
	{
		TArray<int32> ParsedSet;
		if (CurrentDiffLib.Num() > 0)
		{
			FString Example = CurrentDiffLib[FMath::RandRange(0,CurrentDiffLib.Num()-1)]->Pattern;
			for (const wchar_t Elem : Example)
			{
				ParsedSet.Add(_ttoi(&Elem));
			}

			//if datatable doesnt have enough values, rest is filled with zeroes
			for (int i = Example.Len(); i <= PatternSize * PatternSize; i++)
			{
				ParsedSet.Add(0);
			}
		}

		for (FVector2D SpawnPoint : PossiblePoints)
		{  
			int32 BlockPos = SpawnPoint.X * PatternSize + SpawnPoint.Y + 1;		//recount position properly!!

			if (ParsedSet[BlockPos-1] > 0)
			{
					SpawnBlock(FVector(GetActorLocation().X,
						GetActorLocation().Y - 300 * PatternSize + 600 * SpawnPoint.Y + 250,
						GetActorLocation().Z + 300 * PatternSize - 600 * SpawnPoint.X - 250),
						ParsedSet[BlockPos-1] - 1);
			}
		}
	}
	else
	{
		//get from roster random line, if its 1, remove it from library
		//iterate through list of enums to spawn corresponding item in corresponding place.
		//FBreadSlice ChosenSlice = CurrentLibrary[FMath::RandRange(0, CurrentLibrary.Num()-1)];

		//Selecting unique spawn points, more with higher difficulty
		int TargetAmountOfSpawns = 8 + Difficulty;
		TArray<FVector2D> SpawnPoints = PossiblePoints;

		// Allegedly slightly less retarded method compared to the previous one.
		// removing extraneous points from pool, leaving only required amount
		if (PatternSize * PatternSize - TargetAmountOfSpawns > 0)
		{
			for (int i = 0; i < (PatternSize * PatternSize - TargetAmountOfSpawns); i++)
			{
				SpawnPoints.RemoveAt(FMath::RandRange(0, SpawnPoints.Num() - 1));
			}
		}

		// Setting spawn of generic bricks
		for (FVector2D SpawnPoint : SpawnPoints)
		{
			SpawnBlock(FVector(GetActorLocation().X,
				GetActorLocation().Y - 300 * PatternSize + 600 * SpawnPoint.X + 250,
				GetActorLocation().Z - 300 * PatternSize + 600 * SpawnPoint.Y + 250),
				FMath::FRandRange(0, 45) / 15);		// TODO: Needs proper division

		}
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
			NewBonus->SetNewVelocity(InitialBrickVelocity);
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
			Cast<ASimpleBrick>(NewBonus)->SetNewVelocity(InitialBrickVelocity);
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

		//ugly case for processing extended classes
		if (BlockIndex < 2)
		{
			ASimpleBrick* NewBrick = Cast<ASimpleBrick>(Pool->RetrieveActor(BrickSet[BlockIndex], SpawnTransform));

			if (NewBrick)
			{
				NewBrick->Enable();
				NewBrick->SetNewVelocity(InitialBrickVelocity);

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
		else
		{
			AFlyingBrick* NewBrick = Cast<AFlyingBrick>(Pool->RetrieveActor(BrickSet[BlockIndex], SpawnTransform));

			if (NewBrick)
			{
				NewBrick->Enable();
				NewBrick->SetNewVelocity(InitialBrickVelocity);

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
}

//Repopulating pattern set and sending a UI widget
void ABrickGenerator::IncreaseDifficulty(int ExtraDiff)
{
	Difficulty += ExtraDiff;
	LaunchMilestoneAnnouncement(Difficulty);
	AttachNewDifficultySet(Difficulty);
}

//Onetime pattern agregation from datatable
void ABrickGenerator::CollectPatterns()
{
	if (BrickPatternSource)
	{
		BrickPatternSource->GetRowNames();

		for (FName TableRowName : BrickPatternSource->GetRowNames())
		{
			PatternLib.Add(BrickPatternSource->FindRow<FBrickPattern>(TableRowName, "")->Difficulty,
				BrickPatternSource->FindRow<FBrickPattern>(TableRowName, ""));
		}
	}
}

void ABrickGenerator::AttachNewDifficultySet(int NewDifficulty)
{
	CurrentDiffLib.Empty();
	PatternLib.MultiFind(NewDifficulty, CurrentDiffLib);
}

void ABrickGenerator::PurgeEntities()
{
	//requires disabling actors as well, which means a) letting pool know about them or b) halting delegates on disable
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

