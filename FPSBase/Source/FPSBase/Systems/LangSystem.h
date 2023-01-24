#pragma once
#include "CoreMinimal.h"
#include "../Util/JSON/JsonTree.h"


namespace LangSystem {
	
	//Translation is based on a prioritiation, as follows:
	//1. Flavored, localized text
	//2. Unflavored, localized text
	//3. Flavored, English text
	//4. Unflavored, English text
	const FString* Localize(const FString& token, const FactionLangFlavor* flavor = nullptr);

	bool FlavorExists(const FactionLangFlavor* flavor);

	void SetCurrentLang(const LangCode& lang);

	LangCode GetCurrentLang();


	//Use these to add flavors and languages
	//The function smartly inferences flavor and lang based on folder structure
	//Any lang in a folder titled "Lang" is assumed to be unflavored,
	//elsewise it's assumed to be flavored, using the folder name as the flavor name
	void AddLang(JsonTree* pFile);
	
};