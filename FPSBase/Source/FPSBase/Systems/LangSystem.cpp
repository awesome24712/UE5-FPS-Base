
#include "LangSystem.h"
#include "CoreMinimal.h"

namespace LangSystem {
	//Global lang variables

	//Current lang setting
	LangCode g_currentLang;

	//Default language texts excluding flavors
	TMap<LangCode, TMap<FString, FString>> g_defaultLangMap;

	//Flavor-specific per-language text overrides
	TMap<FactionLangFlavor, TMap<LangCode, TMap<FString, FString>>> g_flavorLangMap;

	//Translation is based on a prioritiation, as follows:
	//1. Flavored, localized text
	//2. Unflavored, localized text
	//3. Flavored, English text
	//4. Unflavored, English text
	const FString* Localize(const FString& token, const FactionLangFlavor* flavor) {

		auto englishUnflavored = g_defaultLangMap.Find("en");

		if (flavor && FlavorExists(flavor)) {
			auto flavoredLang = g_flavorLangMap.Find(*flavor)->Find(g_currentLang);

			if (flavoredLang) {
				FString* flavoredTranslation = flavoredLang->Find(token);
				if (flavoredTranslation) return flavoredTranslation;
			}
			else {
				auto defaultLang = g_defaultLangMap.Find(g_currentLang);
				FString* defaultTranslation = defaultLang->Find(token);
				if (defaultTranslation) return defaultTranslation;
			}

			auto englishFlavored = g_flavorLangMap.Find(*flavor)->Find("en");
			if (englishFlavored) {
				auto englishFlavoredTranslation = englishFlavored->Find(token);
				if (englishFlavoredTranslation) return englishFlavoredTranslation;
			}
			else {
				auto englishUnflavoredTranslation = englishUnflavored->Find(token);
				if (englishUnflavoredTranslation) return englishUnflavoredTranslation;
			}
		}
		else {
			auto defaultLang = g_defaultLangMap.Find(g_currentLang);
			FString* defaultTranslation = defaultLang->Find(token);
			if (defaultTranslation) return defaultTranslation;

			auto englishUnflavoredTranslation = englishUnflavored->Find(token);
			if (englishUnflavoredTranslation) return englishUnflavoredTranslation;
		}
		NLogger::Warning("Localizer: Token %s could not be localized", WCStr(*token));
		return &token;
	}

	bool FlavorExists(const FactionLangFlavor* flavor) {
		return g_flavorLangMap.Contains(*flavor);
	}
	bool FlavoredLangExists(const FactionLangFlavor* flavor, LangCode lang) {
		
		return FlavorExists(flavor) && g_flavorLangMap.FindByHash(flavor->Len(), *flavor);
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


	//Use these to add flavors and languages
	//The function smartly inferences flavor and lang based on folder structure
	//Any lang in a folder titled "Lang" is assumed to be unflavored,
	//elsewise it's assumed to be flavored, using the folder name as the flavor name
	void AddLang(JsonTree* pFile) {
		if (!pFile->GetParent() || pFile->GetParent()->GetType() != JNT_FOLDER) {
			NLogger::Warning("Localizer: Cannot parse orphaned JsonTree for AddLang");
			return;
		}

		//First create the map in the right place, then add to it.
		//This is more efficient than creating it and then copying it into place
		const FString& folderName = pFile->GetParent()->Key().ToString();
		bool unflavored = (folderName == "Lang");


	}

	void InitLangSystem() {
		//Get list of folders in Lang folder

		//For each lang folder, load up all JSONs. JSONs in subfolders use subfolder names as FactionLangFlavor names.
		//Sub-sub-folders are ignored.

	}
}