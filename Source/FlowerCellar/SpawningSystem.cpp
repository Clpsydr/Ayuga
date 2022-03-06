#include "SpawningSystem.h"

AActor* USpawningSystem::RetrieveActor(UClass* Class, const FTransform& Transform)
{
	if (!Class)
	{
		return nullptr;
	}

	TArray<AActor*>* Pool = InactiveActors.Find(Class);
	AActor* Result = nullptr;
	if (Pool && Pool->Num() > 0)
	{
		Result = Pool->Pop();
		FVector CorrectedScale = Transform.GetScale3D() * Result->GetActorScale();  //scale correction that isnt actually required
		Result->SetActorTransform(FTransform(Transform.GetRotation(), Transform.GetLocation(), CorrectedScale), false, nullptr,
			ETeleportType::ResetPhysics);
		Result->SetActorTickEnabled(true);
	}
	else
	{
		Result = GetWorld()->SpawnActor(Class, &Transform);
		if (!Result)
		{
			return nullptr;
		}

		ActorArray.Add(Result);
		Result->OnDestroyed.AddDynamic(this, &USpawningSystem::OnActorDestroyed);
	}

	return Result;
}

void USpawningSystem::ReturnActor(AActor* Actor)
{
	if (!Actor || !IsActorInPool(Actor))
	{
		return;
	}

	TArray<AActor*>& Pool = InactiveActors.FindOrAdd(Actor->GetClass());
	if (!Pool.Contains(Actor))
	{
		Actor->SetActorTickEnabled(false);
		Pool.Add(Actor);
	}
}

void USpawningSystem::PurgeActiveActors()
{
	for (auto* Actr : ActorArray)
	{
		
		ReturnActor(Actr);
	}
}

bool USpawningSystem::IsActorInPool(AActor* Actor) const
{
	return ActorArray.Contains(Actor);
}

bool USpawningSystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USpawningSystem::OnActorDestroyed(AActor* Actor)
{
	check(IsActorInPool(Actor));
	TArray<AActor*>* Pool = InactiveActors.Find(Actor->GetClass());
	if (Pool && Pool->Contains(Actor))
	{
		Pool->Remove(Actor);
	}
	ActorArray.Remove(Actor);
}
