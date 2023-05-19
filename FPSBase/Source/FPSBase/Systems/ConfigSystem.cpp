#include "ConfigSystem.h"
#include "ConsoleSystem.h"
#include "../Util/JSON/JsonTree.h"

namespace ConfigSystem {

	//Modifies the give TArray to include all persistent cvars
	void GetArchivableCvars(TArray<TPair<FString, ConVar*>>& out) {
		TArray<TPair<FString, ConVar*>> all = ConVar::GetAllCvars();
		out.Empty();
		for (auto e : all) {
			if (e.Value->HasFlag(CVAR_PERSISTENT)) {
				out.Add(e);
			}
		}
	}

	//Loads the given Json from disk and applies its cvar values
	void LoadConfigFromDisk(const FString& path) {
		JsonTreeHandle jth = JsonTreeHandle::CreateFromFile(path);
		if (!jth) {
			NLogger::Fatal("Could not find %s for loading", CStr(path));
			return;
		}

		for (int i = 0; i < jth->NumChildren(); i++) {
			auto child = jth->GetChild(i);
			if (ConVar* cvar = ConVar::FindCvarByName(child->Key())) {
				//set value using type from json
				switch (child->GetType()) {
				case JNT_BOOLEAN:
					cvar->SetValue(child->GetValueBool());
					break;
				case JNT_DOUBLE:
					cvar->SetValue((float)child->GetValueDouble());
					break;
				case JNT_DOUBLE_TO_INT:
					cvar->SetValue((int)child->GetValueDouble());
					break;
				case JNT_STRING:
					cvar->SetValue(child->GetValueString());
					break;
				default:
					NLogger::Warning("While loading a config cvar %s set to bad type %i, skipping...", 
						CStr(child->Key()), 
						(int)child->GetType());
				}
			}
			else if (ConCommand* cmd = ConCommand::FindCommandByName(child->Key())) {
				
			}
			else {
				NLogger::Warning("Unknown variable or command %s, skipping...", CStr(child->Key()));
			}
		}
	}

	//Loads config.json from disk and applies its cvar values
	void LoadDefaultConfigFromDisk() {
		LoadConfigFromDisk("config.json");
	}

	void WriteConfigToDisk() {

	}

	void LoadAutoexecFromDisk() {
		LoadConfigFromDisk("autoexec.json");
	}
}

CON_COMMAND(exec, CVAR_MOD, "Runs the given json file as a config") {
	if (argc != 2) {
		NLogger::Fatal("Invalid number of arguments");
	}

	ConfigSystem::LoadConfigFromDisk(args[1]);
}