// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveUpActor.h"
#include "Util/FileIO/Logger.h"
#include "Util/JSON/JsonTree.h"
//#include "Components/SphereComponent.h"


// Sets default values
AMoveUpActor::AMoveUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/WorldProps/dino_puppet.dino_puppet'"));
	UStaticMesh* Asset = MeshAsset.Object;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoveUpMeshComp"));
	meshComponent->SetStaticMesh(Asset);

	RootComponent = meshComponent;

	JT_START_BINDING_UCLASS("MoveUpActor", AMoveUpActor);
	JT_BIND_DOUBLE(m_moveUpDelta, "moveUpDelta", true);
	JT_BIND_DOUBLE(m_scaleChange, "apples", true);
	JT_FINISH_BINDING();

}

// Called when the game starts or when spawned
void AMoveUpActor::BeginPlay()
{
	Super::BeginPlay();

	//NLogger::Fatal("double variable location at %p\n", &m_moveUpDelta);

	LoadBindingsFromJson("moveup.json");
}

// Called every frame
void AMoveUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector loc = GetActorLocation();
	loc.Z += m_moveUpDelta;
	SetActorLocation(loc);

	FVector scale = GetActorScale3D();
	scale.X += m_scaleChange;
	scale.Y += m_scaleChange;
	scale.Z += m_scaleChange;
	SetActorScale3D(scale);
}


