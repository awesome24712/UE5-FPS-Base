#pragma once

#include "CoreMinimal.h"
#include "Flag.h"
#include "../Util.h"
#include "SkiFlag.generated.h"

UCLASS(config = Game)
class ASkiFlag : public AFlag, public IJsonBindable {
	GENERATED_BODY()

protected:

public:

	ASkiFlag();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;

};