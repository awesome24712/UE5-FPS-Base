// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/JSON/PropertyBinder.h"
#include "MoveUpActor.generated.h"

class USphereComponent;

UCLASS()
class FPSBASE_API AMoveUpActor : public AActor, public IJsonBindable
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(EditAnywhere, Category = Projectile)
	UStaticMeshComponent* meshComponent;
	
public:	
	// Sets default values for this actor's properties
	AMoveUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	FVector m_tickTransform = FVector();

	double m_moveUpDelta = 1.0;
	double m_scaleChange = 0.01;
};
