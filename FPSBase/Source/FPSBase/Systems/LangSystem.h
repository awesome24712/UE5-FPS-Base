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


	void InitLangSystem();
	
};