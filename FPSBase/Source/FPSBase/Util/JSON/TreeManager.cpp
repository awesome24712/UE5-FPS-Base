#include "TreeManager.h"
#include "CoreMinimal.h"
#include "../../Util.h"

namespace NTreeManager {
	static JsonTreeHandle* g_pTopLevelTree = nullptr;

	//loads Mods folder into one giant JsonTreeHandle
	void LoadTopLevelTree() {
		if (g_pTopLevelTree) {
			delete g_pTopLevelTree;
		}
		JsonTreeHandle top = JsonTreeHandle::CreateFromDirectory(FString(""));
		g_pTopLevelTree = new JsonTreeHandle(top); //make a copy
	}

	//might return null
	const JsonTree* FindFile(const FString& fileName) {
		if (!g_pTopLevelTree) {
			Log("ERROR g_TopLevelTree is NULL when trying to load %s", WCStr(fileName));
			return nullptr;
		}

		return g_pTopLevelTree->Get()->BFS(JNT_FILE, fileName);
	}

	//might return null
	const JsonTree* FindFolder(const FString& folderName) {
		if (!g_pTopLevelTree) {
			Log("ERROR g_TopLevelTree is NULL when trying to load %s", WCStr(folderName));
			return nullptr;
		}

		return g_pTopLevelTree->Get()->BFS(JNT_FOLDER, folderName);
	}

	void LoadBindablesFromFile(IJsonBindable* target, const FString& fileName) {
		auto tree = FindFile(fileName);
		Log(CStr(tree->ToString()));
		if (tree) {
			target->LoadBindingsFromJson(tree);
		}
	}
}