#pragma once
#include "CoreMinimal.h"
#include "../Util/FileIO/Logger.h"

namespace ConfigSystem {

	//Loads the given Json from disk and applies its cvar values
	void LoadConfigFromDisk(const FString& path);

	//Loads config.json from disk and applies its cvar values
	void LoadDefaultConfigFromDisk();

	void WriteConfigToDisk();

	void LoadAutoexecFromDisk();


}