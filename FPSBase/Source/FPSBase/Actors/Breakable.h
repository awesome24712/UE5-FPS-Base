#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Util.h"
#include "Breakable.generated.h"

UCLASS(config = Game)
class ABreakable : public AActor, public IJsonBindable {
	GENERATED_BODY()

protected:

public:

	ABreakable();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;

	bool IsEnabled() const { return IsActorTickEnabled(); }
	void SetEnabled(bool bEnabled) { SetActorTickEnabled(bEnabled); }
};