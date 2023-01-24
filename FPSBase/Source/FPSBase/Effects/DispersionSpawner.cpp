// Fill out your copyright notice in the Description page of Project Settings.


#include "DispersionSpawner.h"
#include "../MoveUpActor.h"
#include "../Util/FileIO/Logger.h"
#include "../Util/JSON/JsonTree.h"
#include "../Util/JSON/PropertyBinder.h"

// Sets default values
ADispersionSpawner::ADispersionSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADispersionSpawner::BeginPlay()
{
	Super::BeginPlay();


	NLogger::OpenLogFile();
	NLogger::Log(__FUNCTION__ "\n");


	JsonTreeHandle jth = JsonTreeHandle::CreateFromFile("settings.json");

	if (jth) Msg(jth->ToString());

	if (jth && jth->NumChildren() == 1) {
		const JsonTree* list = jth->GetChild(0);
		for (int i = 0; i < list->NumChildren(); i++) {
			const JsonTree* jc = list->GetChild(i);

			UWorld* const World = GetWorld();
			if (World != nullptr && jc->NumChildren() == 3) {

				AMoveUpActor* pActor = World->SpawnActor<AMoveUpActor>(AMoveUpActor::StaticClass(), GetActorLocation(), GetActorRotation());
				if (pActor) {
					FVector transform(jc->GetChild(0)->GetValueDouble(), jc->GetChild(1)->GetValueDouble(), jc->GetChild(2)->GetValueDouble());
					//Msg("Spawning AMoveUpActor with transform %f %f %f", transform.X, transform.Y, transform.Z);
					pActor->m_tickTransform = transform;
				}
				else {
					NLogger::Fatal("Failed to spawn AMoveUpActor");
				}
			}
		}

		//NLogger::Fatal(jth->ToString());

		//NLogger::Fatal("%p %i %i", jth.Get(), jth->NumChildren(), (int)jth->GetType());
		//NLogger::Fatal("%p", jth->GetChild(0)->GetParent());
	}
	else {
		NLogger::Fatal("JsonTreeHandle for settings.json is NULL!");
	}

	NLogger::CloseLogFile();
	
}

// Called every frame
void ADispersionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

