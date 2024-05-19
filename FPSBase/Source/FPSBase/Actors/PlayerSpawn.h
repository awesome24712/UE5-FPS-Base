#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Util.h"
#include "PlayerSpawn.generated.h"

UCLASS(config = Game)
class APlayerSpawn : public AActor, public IJsonBindable {
	GENERATED_BODY()

protected:

public:

	APlayerSpawn();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;

	bool IsEnabled() const { return IsActorTickEnabled(); }
	void SetEnabled(bool bEnabled) { SetActorTickEnabled(bEnabled); }
};