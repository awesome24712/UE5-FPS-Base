#pragma once

#include "CoreMinimal.h"
#include "BoxTrigger.h"
#include "../Util.h"
#include "PushZone.generated.h"

UCLASS(config = Game)
class APushZone : public AActor, public IJsonBindable {
	GENERATED_BODY()

protected:

public:

	APushZone();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;
};