// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleBrick.h"
#include "GameFramework/Actor.h"
#include "Sunray.generated.h"

UCLASS()
class FLOWERCELLAR_API ASunray : public ASimpleBrick
{
	GENERATED_BODY()
	
public:	
	ASunray();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BonusHitBox;

	UFUNCTION()
		virtual void OnRayStartOverlap(class UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnRayEndOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex);

public:	
	virtual void Tick(float DeltaTime) override;

};
