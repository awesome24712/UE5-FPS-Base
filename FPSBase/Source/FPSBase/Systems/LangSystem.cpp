
#include "LangSystem.h"
#include "CoreMinimal.h"
#include "../Util.h"

namespace LangSystem {
	//Global lang variables

	//Current lang setting
	LangCode g_currentLang = "en";

	//Default language texts excluding flavors
	TMap<LangCode, TMap<FString, FString>> g_defaultLangMap;

	//Flavor-specific per-language text overrides
	TMap<FactionLangFlavor, TMap<LangCode, TMap<FString, FString>>> g_flavorLangMap;

	//Translation is based on a prioritiation, as follows:
	//1. Flavored, localized text
	//2. Unflavored, localized text
	//3. Flavored, English text
	//4. Unflavored, English text
	const FString& Localize(const FString& token, const FactionLangFlavor& flavor) {

		auto englishUnflavored = g_defaultLangMap.Find("en");

		if (!flavor.IsEmpty() && FlavorExists(flavor)) {
			auto flavoredLang = g_flavorLangMap.Find(flavor)->Find(g_currentLang);

			if (flavoredLang) {
				FString* flavoredTranslation = flavoredLang->Find(token);
				if (flavoredTranslation) return *flavoredTranslation;
			}
			else {
				auto defaultLang = g_defaultLangMap.Find(g_currentLang);
				FString* defaultTranslation = defaultLang->Find(token);
				if (defaultTranslation) return *defaultTranslation;
			}

			auto englishFlavored = g_flavorLangMap.Find(flavor)->Find("en");
			if (englishFlavored) {
				auto englishFlavoredTranslation = englishFlavored->Find(token);
				if (englishFlavoredTranslation) return *englishFlavoredTranslation;
			}
			else {
				auto englishUnflavoredTranslation = englishUnflavored->Find(token);
				if (englishUnflavoredTranslation) return *englishUnflavoredTranslation;
			}
		}
		else {
			auto defaultLang = g_defaultLangMap.Find(g_currentLang);
			if (!defaultLang) { NLogger::Fatal("ERROR couldn't find lang data for current LangCode!\n"); return token; }
			FString* defaultTranslation = defaultLang->Find(token);
			if (defaultTranslation) return *defaultTranslation;

			auto englishUnflavoredTranslation = englishUnflavored->Find(token);
			if (englishUnflavoredTranslation) return *englishUnflavoredTranslation;
		}
		NLogger::Warning("Localizer: Token %s could not be localized\n", CStr(token));
		return token;
	}

	bool FlavorExists(const FactionLangFlavor& flavor) {
		return g_flavorLangMap.Contains(flavor);
	}
	bool FlavoredLangExists(const FactionLangFlavor& flavor, LangCode lang) {
		
		return FlavorExists(flavor) && g_flavorLangMap.FindByHash(flavor.Len(), flavor);
	}

	void SetCurrentLang(const LangCode& lang) {
		if (g_defaultLangMap.Contains(lang)) {
			g_currentLang = lang;
		}
		else {
			NLogger::Fatal("Attempt to switch to non-existant language with LangCode %s", WCStr(*lang));
		}
		
	}

	LangCode GetCurrentLang() {
		return g_currentLang;
	}

	//helper function for the function beneath it
	void AddFolderContentsToMap(TMap<FString, FString>& map, const JsonTree* pFolder) {
		//loop through all files and their contents
		for (int f = 0; f < pFolder->NumChildren(); f++) {
			const JsonTree* pFile = pFolder->GetChild(0);
			//if our file is actually a folder, that's a flavor folder, ignore it
			if (pFile->GetType() == JNT_FOLDER) {
				continue;
			}

			for (int i = 0; i < pFile->NumChildren(); i++) {
				//now process each mapping, adding it to the map
				const JsonTree* pMapping = pFile->GetChild(i);
				if (pMapping->GetType() != JNT_STRING) {
					NLogger::Fatal("Encountered non-string type %i when parsing lang file, ignoring", pFile->GetType());
					continue; //check the next mapping
				}
				map.Add(pMapping->Key(), pMapping->GetValueString());
			}
		}
	}

	//Use these to add flavors and languages
	//The function smartly inferences flavor and lang based on folder structure
	//Any lang in a folder titled "Lang" is assumed to be unflavored,
	//elsewise it's assumed to be flavored, using the folder name as the flavor name
	void AddLangFolder(const JsonTree* pFolder) {

		if (!pFolder->GetParent() || pFolder->GetParent()->GetType() != JNT_FOLDER) {
			NLogger::Warning("Localizer: Cannot parse orphaned JsonTree for AddLang");
			return;
		}

		//determine lang code and flavored vs unflavored
		const FString& parentFolderName = pFolder->GetParent()->Key();
		bool unflavored = (parentFolderName == "Langs");
		FString langCode;
		FString flavor = "";
		if (unflavored) {
			langCode = pFolder->Key();

			//Find map or create it if it doesn't exist
			if (!g_defaultLangMap.Contains(langCode)) {
				g_defaultLangMap.Add(langCode, TMap<FString, FString>());
			}
			TMap<FString, FString>& map = *(g_defaultLangMap.Find(langCode));
			AddFolderContentsToMap(map, pFolder);
		}
		else {
			langCode = pFolder->GetParent()->Key();
			flavor = pFolder->Key();

			//Find map or create it if it doesn't exist
			if (!g_flavorLangMap.Contains(flavor)) {
				g_flavorLangMap.Add(flavor, TMap<LangCode, TMap<FString, FString>>());
			}
			auto langMap = *(g_flavorLangMap.Find(langCode));
			if (!langMap.Contains(langCode)) {
				langMap.Add(langCode, TMap<FString, FString>());
			}

			TMap<FString, FString>& map = *(langMap.Find(langCode));
			AddFolderContentsToMap(map, pFolder);
		}
	}

	void InitLangSystem() {
		//Get list of folders in Lang folder
		const JsonTree* langFolder = NTreeManager::FindFolder("Langs");
		if (!langFolder) {
			NLogger::Fatal("Couldn't load Langs folder\n");
			return;
		}

		//For each lang folder, load up all JSONs. JSONs in subfolders use subfolder names as FactionLangFlavor names.
		//Sub-sub-folders are ignored.
		for (int f = 0; f < langFolder->NumChildren(); f++) {
			const JsonTree* pFolder = langFolder->GetChild(f);
			AddLangFolder(pFolder);

			//also add any immediate subfolders. AddLangFolder will detect it as a flavor
			for (int i = 0; i < pFolder->NumChildren(); i++) {
				const JsonTree* pFileOrFlavor = pFolder->GetChild(i);
				if (pFileOrFlavor->GetType() == JNT_FOLDER) {
					AddLangFolder(pFileOrFlavor);
				}
			}
		}
	}
}