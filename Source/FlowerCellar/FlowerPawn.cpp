#include "FlowerPawn.h"
#include "FlowerPController.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

AFlowerPawn::AFlowerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Unit Body"));
	RootComponent = BodyMesh;

	Abil1Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hardened form"));
	Abil1Mesh->SetupAttachment(BodyMesh);

	Abil2Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flytrap form"));
	Abil2Mesh->SetupAttachment(BodyMesh);

	CollideSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion Sound"));
	CollideSoundEffect->SetupAttachment(BodyMesh);


	HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit collider"));
	HitBox->SetupAttachment(BodyMesh);

	StemParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Stem effect"));
	StemParticleEffect->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AFlowerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	Abil1Mesh->SetHiddenInGame(true);
	Abil2Mesh->SetHiddenInGame(true);
}

void AFlowerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// movement
	FVector MovePosition = GetActorLocation() + MoveSpeed * MovementDirection * DeltaTime;
	SetActorLocation(MovePosition, false);

	/// marginal stat updates
	if (bIsSustained)
	{
		LifeForce += DeltaTime * 9;
	}
	else
	{
		LifeForce -= DeltaTime;
	}
	
}

void AFlowerPawn::ResetParams()
{
	Resource = 0;
	LifeForce = MaxLifeForce;
	Petals = MaxPetals;
	RegenerateVisualPetals();
}

void AFlowerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFlowerPawn::VMovement(float AxisValue)
{
	MovementDirection.Z = AxisValue;
}

void AFlowerPawn::HMovement(float AxisValue)
{
	MovementDirection.Y = AxisValue;
}

void AFlowerPawn::GainItem(int32 Amount)
{
	(Resource + Amount < MaxResource) ? Resource += Amount : Resource = MaxResource;
}

void AFlowerPawn::UseItem()
{
	if (Resource < 4)
	{
		return;
	}

	if (Resource > 9)
	{
		SecondAbility();
		Resource -= 10;
	}
	else 
	{
		FirstAbility();
		Resource -= 5;
	}
}

// gaining onetime effect of breaking through walls of defence 1
void AFlowerPawn::FirstAbility()
{
	bIsReinforced = true;
	BodyMesh->SetHiddenInGame(true, false);
	Abil1Mesh->SetHiddenInGame(false);
	//switch model
}

// gaining onetime effect of breaking through edible wals (ie flies)
void AFlowerPawn::SecondAbility()
{
	bIsCarnivorous = true;
	BodyMesh->SetHiddenInGame(true, false);
	Abil2Mesh->SetHiddenInGame(false);
	//switch model
}

void AFlowerPawn::ReceiveDamage(int DamageAmount)
{
	if (DamageAmount < 0)		//droplet process
	{
		LifeForce += 1.f;
		GainItem(2);
		return;
	}

	if (bIsCarnivorous && DamageAmount > 1)		// flytrap effect process
	{
		LifeForce += 2.f;

		bIsCarnivorous = false;
		BodyMesh->SetHiddenInGame(false, false);
		Abil2Mesh->SetHiddenInGame(true);
		return;
	}

	if (bIsReinforced && DamageAmount < 2)		// armored effect process
	{	
		bIsReinforced = false;
		BodyMesh->SetHiddenInGame(false, false);
		Abil1Mesh->SetHiddenInGame(true);
		return;
	}

	if (bIsVulnerable)
	{
		Petals -= DamageAmount;
		CollideSoundEffect->Play();

		RegenerateVisualPetals();
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bIsVulnerable = false;
		GetWorld()->GetTimerManager().SetTimer(IFrameHandle, this, &AFlowerPawn::ResetVulnerability, InvulTime, false);
	}
}

void AFlowerPawn::ResetVulnerability()
{
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsVulnerable = true;
}

void AFlowerPawn::SunBathe(bool BeginEffect)
{
	bIsSustained = BeginEffect;
}

void AFlowerPawn::RegenerateVisualPetals()
{
	if (PetalModels.Num() > Petals)
	{
		int imodels = PetalModels.Num() - FMath::Clamp(Petals, 0, Petals);
		for (int i = 0; i < imodels; i++)
		{
			AActor* DyingPetal = PetalModels.Last();
			DyingPetal->Destroy();
			PetalModels.RemoveAt(PetalModels.Num() - 1);
		}
	}
	else
	{
		int imodels = PetalModels.Num();
		for (int i = 0; i < Petals - imodels; i++)
		{
			AActor* NewPetal;

			NewPetal = GetWorld()->SpawnActor<AActor>(PetalType,
				GetActorLocation(),
				FRotator(0.f, 0.f, 0.f));
			NewPetal->AttachToComponent(BodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
			PetalModels.Add(NewPetal);
		}
	}

	for (auto* PetalModel : PetalModels)
	{
		PetalModel->SetActorRelativeRotation(FRotator(180.f - PetalModels.Find(PetalModel) * 60.f, 90.f, 90.f));
	}
}
