#include "PerFrameSystemManager.h"


static TArray<PerFrameSystem*> g_perFrameSystems;

PerFrameSystem::PerFrameSystem(const FString& name) : m_name(name) {
	g_perFrameSystems.Push(this);
}

void PerFrameSystem::InitAll() {
	for (auto sys : g_perFrameSystems) {
		sys->Init();
	}
}

void PerFrameSystem::TickAll(float deltaSeconds) {
	for (auto sys : g_perFrameSystems) {
		sys->Tick(deltaSeconds);
	}
}
