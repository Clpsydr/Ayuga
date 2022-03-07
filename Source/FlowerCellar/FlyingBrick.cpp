#include "FlyingBrick.h"
#include "FlowerPawn.h"
#include "Components/BoxComponent.h"

AFlyingBrick::AFlyingBrick()
{
	if (Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass())))
	{
		Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()))->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);
	}
}

void AFlyingBrick::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AFlowerPawn* PresumedPlayer = Cast<AFlowerPawn>(OtherActor);
	if (!PresumedPlayer)
	{
		FlyDirection *= -1;
	}
}

/// rotate initially either upwards or sideways with +-
/// on collision flip front vector 180 degrees
void AFlyingBrick::BeginPlay()
{
	Super::BeginPlay();

	if (FMath::RandRange(0, 1) > 0)
	{
		FlyDirection = FVector(0.f, 1.f, 0.f);
	}
	else
	{
		FlyDirection = FVector(0.f, 0.f, 1.f);
	}
}

void AFlyingBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MovePosition = GetActorLocation() + FlyVelocty * FlyDirection * DeltaTime;
	SetActorLocation(MovePosition, true);
}
