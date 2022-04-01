#include "SimpleBrick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FlowerPawn.h"
#include "SpawningSystem.h"


ASimpleBrick::ASimpleBrick()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Unit Body"));
	RootComponent = BodyMesh;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitBox->SetupAttachment(BodyMesh);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASimpleBrick::OnMeshOverlapBegin);
	
/*	DestroyParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Stem effect"));
	DestroyParticleEffect->SetupAttachment(BodyMesh); */
}

void ASimpleBrick::BeginPlay()
{
	Super::BeginPlay();
}

void ASimpleBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MovePosition = GetActorLocation() + FallingSpeed * FVector(-1.f, 0.f, 0.f) * DeltaTime;

	SetActorLocation(MovePosition, true);

	// very dumb, change this
	if (bIsSpinning)
	{
		BodyMesh->SetWorldRotation(FMath::RInterpConstantTo(BodyMesh->GetComponentRotation(),
			FRotator(BodyMesh->GetComponentRotation().Pitch + 2.f,
				BodyMesh->GetComponentRotation().Roll,
				BodyMesh->GetComponentRotation().Yaw),
			GetWorld()->GetDeltaSeconds(),
			45.f));
	}
}

void ASimpleBrick::ApplyEffect(AActor* HitActor)
{
	AFlowerPawn* PresumedPlayer = Cast<AFlowerPawn>(HitActor);
	if (PresumedPlayer)
	{
		PresumedPlayer->ReceiveDamage(Damage);
	}
}

void ASimpleBrick::Enable()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	BodyMesh->SetHiddenInGame(false);
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASimpleBrick::Disable()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	BodyMesh->SetHiddenInGame(true);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	USpawningSystem* BulletPool = GetWorld()->GetSubsystem<USpawningSystem>();
	if (BulletPool->IsActorInPool(this))
	{
		BulletPool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}

void ASimpleBrick::SetNewVelocity(float NewVelocity)
{
	FallingSpeed = NewVelocity;
}

void ASimpleBrick::Accelerate()
{
	FallingSpeed += FMath::RandRange(1200.f, 1400.f);
	bIsSpinning = true;
}

void ASimpleBrick::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ApplyEffect(OtherActor);
	Disable();
}