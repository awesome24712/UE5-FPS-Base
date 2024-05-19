#pragma once

#include "CoreMinimal.h"
#include "Flag.h"
#include "../Util.h"
#include "CtfFlag.generated.h"

UCLASS(config = Game)
class ACtfFlag : public AFlag, public IJsonBindable {
	GENERATED_BODY()

protected:

public:

	ACtfFlag();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;

};