#pragma once

#include "../Profiles/LangProfile.h"
#include "../Profiles/MapListingProfile.h"
#include "../Profiles/MapProfile.h"
#include "../Profiles/MenuItemProfile.h"
#include "../Profiles/MenuProfile.h"
#include "../Profiles/MissionProfile.h"
#include "../Profiles/ModelProfile.h"
#include "../Profiles/PlayerCharacterProfile.h"
#include "../Profiles/PlayerProgressionProfile.h"

#include "../Profiles/PropProfile.h"

#include "../Profiles/SettingsProfile.h"
#include "../Profiles/SoundProfile.h"


#include "../Profiles/VoiceLineProfile.h"
#include "../Util/FileIO/Logger.h"



class ModProfile : public IJsonBindable {
public:
	FString m_displayName;
	FString m_imagePath;
	TArray<FString> m_credits; //"Lead: Awesome", "Models: Awesome", etc.
	bool m_storyOverride; //should this mod override the story with its own?
	bool m_storyDisabled; //should this mod disable all story elements?
	bool m_hubWorldOverride; //does this mod replace the hub world?

	ModProfile();
};

class SystemManager
{
private:
	TArray<ModProfile*> m_modList;
	TArray<ModProfile*> m_activeModList; //list of mods currently active

	JsonTreeHandle m_pLoadedModContent; //top-level JSON tree representing all JSONs for all mods currently loaded. Root is Mods folder.
	

public:
	SystemManager();
	~SystemManager();
	

	void						LoadModProfileList(); //loads all mod profiles for the purpose of display them
	void						LoadSelectedModProfileContent(); //loads selected mod content into memory - heavy duty function!
	
	void						SetSelectedModProfileList(const TArray<ModProfile*>& list); //call this when user is finished selecting mods from menu

	const TArray<ModProfile*>*	GetModProfileList() const { return &m_modList; };
	const TArray<ModProfile*>*	GetSelectedModProfileList() const { return &m_modList; };

	void						InitializeAllSystems();

};

extern SystemManager* g_pSystems;

