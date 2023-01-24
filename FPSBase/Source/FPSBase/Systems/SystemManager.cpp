#include "SystemManager.h"
#include "../Util/JSON/PropertyBinder.h"
#include ""

SystemManager* g_pSystems;

ModProfile::ModProfile() {

	//JT_START_BINDING("mod", ModProfile)

}

SystemManager::SystemManager() {
	g_pSystems = this;
}

SystemManager::~SystemManager() {
	g_pSystems = this;
}

void SystemManager::LoadModProfileList()
{
	
}

void SystemManager::LoadSelectedModProfileContent()
{

}

void SystemManager::SetSelectedModProfileList(const TArray<ModProfile*>& list)
{

}

void SystemManager::InitializeAllSystems() {

}

