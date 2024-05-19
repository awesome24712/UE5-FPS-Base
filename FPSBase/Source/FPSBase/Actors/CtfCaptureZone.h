#pragma once

#include "CoreMinimal.h"
#include "BoxTrigger.h"
#include "../Util.h"
#include "CtfCaptureZone.generated.h"

UCLASS(config = Game)
class ACtfCaptureZone : public ABoxTrigger, public IJsonBindable {
	GENERATED_BODY()

protected:

public:

	ACtfCaptureZone();

	virtual void BeginPlay();

	
};