#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlowerPawn.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class UParticleSystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UAudioComponent;

/*
*  Main player controlled pawn in the game
*/
UCLASS()
class FLOWERCELLAR_API AFlowerPawn : public APawn
{
	GENERATED_BODY()

public:
	AFlowerPawn();

private:
	bool bIsVulnerable = true;

	bool bIsCarnivorous = false;

	bool bIsReinforced = false;

	bool bIsSustained = false;

	TArray<AActor*> PetalModels;

	int32 Resource = 0;

	int32 Petals = 6;

	float LifeForce = 600.f;

/// spatial

	FVector MovementDirection;

	FTimerHandle IFrameHandle;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Abil1Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Abil2Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UAudioComponent* CollideSoundEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCapsuleComponent* HitBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* StemParticleEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HP visuals")
		TSubclassOf<AActor> PetalType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		float InvulTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		float MoveSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		int32 MaxPetals = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		int32 MaxResource = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
		float MaxLifeForce = 600.f;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void VMovement(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void HMovement(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void GainItem(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void UseItem();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SunBathe(bool BeginEffect);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void FirstAbility();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void SecondAbility();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void ResetParams();



	UFUNCTION(BlueprintCallable, Category = "Actions")
		void ReceiveDamage(int32 DamageAmount);

	UFUNCTION()
		const float GetLifeForce() { return LifeForce; }

	UFUNCTION()
		const float GetPetals() { return Petals; }

	UFUNCTION()
		const float GetResourceRatio() { return float(Resource) / float(MaxResource); }

	UFUNCTION()
		void RegenerateVisualPetals();

	UFUNCTION()
		void ResetVulnerability();
};
