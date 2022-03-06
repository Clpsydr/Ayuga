#include "Sunray.h"
#include "Components/BoxComponent.h"
#include "FlowerPawn.h"

ASunray::ASunray()
{
	PrimaryActorTick.bCanEverTick = true;

	BonusHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("exposure zone"));
	BonusHitBox->SetupAttachment(BodyMesh);
	BonusHitBox->OnComponentEndOverlap.AddDynamic(this, &ASunray::OnRayEndOverlap);
	BonusHitBox->OnComponentBeginOverlap.AddDynamic(this, &ASunray::OnRayStartOverlap);
}

void ASunray::BeginPlay()
{
	Super::BeginPlay();
}

void ASunray::OnRayStartOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlowerPawn* PresumedPlayer = Cast<AFlowerPawn>(OtherActor);
	if (PresumedPlayer)
	{
		PresumedPlayer->SunBathe(true);
	}
	//send bonus to overlapped actor?
}

void ASunray::OnRayEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFlowerPawn* PresumedPlayer = Cast<AFlowerPawn>(OtherActor);
	if (PresumedPlayer)
	{
		PresumedPlayer->SunBathe(false);
	}
}

void ASunray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

