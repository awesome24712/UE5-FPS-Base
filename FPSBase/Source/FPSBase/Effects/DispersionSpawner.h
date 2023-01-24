// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DispersionSpawner.generated.h"

UCLASS()
class FPSBASE_API ADispersionSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMoveUpActor> SpawnedClass;
	
public:	
	// Sets default values for this actor's properties
	ADispersionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
