#include "ConfigSystem.h"
#include "ConsoleSystem.h"
#include "../Util/JSON/JsonTree.h"

namespace ConfigSystem {

	//Modifies the give TArray to include all persistent cvars
	void GetArchivableCvars(TArray<TPair<FString, ConVar*>>& out) {
		TArray<TPair<FString, ConVar*>> all; 
		ConVar::GetAllCvars(all);
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
				case JNT_DOUBLE_TO_FLOAT:
					cvar->SetValue((float)child->GetValueDouble());
					break;
				case JNT_DOUBLE_TO_BYTE:
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

		//get a list of all cvars
		TArray<TPair<FString, ConVar*>> vars;
		ConVar::GetAllCvars(vars);

		//remove all non-persistent cvars from the list
		for (int i = 0; i < vars.Num();) {
			if (!vars[i].Value->HasFlag(CVAR_PERSISTENT)) {
				vars.RemoveAt(i);
			} else {
				i++;
			}
		}

		//build a json tree representation
		JsonTreeHandle jth = JsonTreeHandle::CreateEmptyForEditing();
		JsonTree* jt = jth.GetForEditing();
		jt->ReserveChildren(vars.Num());
		for (auto pair : vars) {
			JsonTree* child = jt->AddChild(pair.Key);
			switch (pair.Value->GetType()) {
			case EConVarType::INT:
				child->SetValue((double)pair.Value->GetValueInt());
				child->SetType(JNT_DOUBLE_TO_INT);
				break;
			case EConVarType::STRING:
				child->SetValue(FString(pair.Value->GetValuePszString()));
				child->SetType(JNT_STRING);
				break;
			case EConVarType::BOOL:
				child->SetValue(pair.Value->GetValueBool());
				child->SetType(JNT_BOOLEAN);
				break;
			case EConVarType::FLOAT:
				child->SetValue((double)pair.Value->GetValueFloat());
				child->SetType(JNT_DOUBLE);
				break;
			}
		}

		//get String representation
		FString stringRep = jth->ToString();

		//Write the file
		FString writePath = FPaths::ProjectModsDir() + "config.json";
		/*FFileHelpers::SaveStringToFile(
			stringRep,
			writePath,
			EEncodingOptions::AutoDetect,
			IFileManager* FileManager,
			uint32 WriteFlags
		);*/
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

CON_COMMAND(bind_save, 0, "Saves the user's current settings") {
	ConfigSystem::WriteConfigToDisk();
}