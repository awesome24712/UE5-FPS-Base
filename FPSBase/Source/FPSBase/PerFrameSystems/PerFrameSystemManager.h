#pragma once
#include "CoreMinimal.h"

class PerFrameSystem {
	FString m_name;

public:
	PerFrameSystem(const FString& name);

	virtual void Init() = 0;
	virtual void Tick(float deltaSeconds) = 0;

	static void InitAll();
	static void TickAll(float deltaSeconds);
};



