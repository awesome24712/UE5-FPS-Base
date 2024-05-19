#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Util.h"
#include "BoxTrigger.generated.h"

UCLASS(config = Game)
class ABoxTrigger : public AActor {
	GENERATED_BODY()

protected:

public:

	ABoxTrigger();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;

	bool IsEnabled() const { return IsActorTickEnabled(); }
	void SetEnabled(bool bEnabled) { SetActorTickEnabled(bEnabled); }
};