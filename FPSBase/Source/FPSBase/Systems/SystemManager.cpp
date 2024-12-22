#include "SystemManager.h"
#include "../Util.h"
#include "./LangSystem.h"

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

//void SystemManager::LoadModProfileList()
//{
//	
//}
//
//void SystemManager::LoadSelectedModProfileContent()
//{
//
//}
//
//void SystemManager::SetSelectedModProfileList(const TArray<ModProfile*>& list)
//{
//
//}

void SystemManager::InitializeAllSystems() {

	//load data from disk first
	NTreeManager::LoadTopLevelTree();

	LangSystem::InitLangSystem();
}

