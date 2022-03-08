#include "FlyingBrick.h"
#include "FlowerPawn.h"
#include "Components/StaticMeshComponent.h"

AFlyingBrick::AFlyingBrick()
{
	if (Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()))->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);
	}
}

void AFlyingBrick::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

AFlowerPawn* PresumedPlayer = Cast<AFlowerPawn>(OtherActor);
	if (!PresumedPlayer)
	{
		TurnAround();
	}
}

/// rotate initially either upwards or sideways with +-
/// on collision flip front vector 180 degrees
void AFlyingBrick::BeginPlay()
{
	Super::BeginPlay();
	FRotator NewRot;

	if (FMath::RandRange(0, 1) > 0)
	{
		NewRot = FRotator(0, 90, 0);
		FlyDirection = FVector::RightVector;
	}
	else
	{
		NewRot = FRotator(0, 0, 90);
		FlyDirection = FVector::UpVector;
	}

	FQuat QuatRot = FQuat(NewRot);
	AddActorWorldRotation(QuatRot, false, 0, ETeleportType::None);
}

void AFlyingBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MovePosition = GetActorLocation() + FlyVelocty * FlyDirection * DeltaTime;
	SetActorLocation(MovePosition, true);
}

void AFlyingBrick::TurnAround()
{
	FRotator NewRot = FRotator(180, 0, 0);
	FQuat QuatRot = FQuat(NewRot);
	AddActorWorldRotation(QuatRot, false, 0, ETeleportType::None);

	FlyDirection *= -1;
}