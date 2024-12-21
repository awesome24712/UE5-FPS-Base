#pragma once
#include "CoreMinimal.h"
#include "PropertyBinder.h"

namespace NTreeManager {
	//loads Mods folder into one giant JsonTreeHandle
	void LoadTopLevelTree(); 

	//might return null
	const JsonTree* FindFile(const FString& fileName); 

	//might return null
	const JsonTree* FindFolder(const FString& folderName); 

	//type T must inherit from IJsonBindable
	//also, any pre-existing T* is freed from memory
	template<class T> 
	void LoadBindablesFromFolder(TArray<T*>& outResult, const FString& folderName) {
		const JsonTree* folder = FindFolder(folderName);
		if (!folder) {
			return;
		}

		//delete any pre-existing data
		for (int i = 0; i < outResult.Num(); i++) {
			delete outResult[i];
		}

		outResult = TArray<T*>();

		for (int i = 0; i < folder->NumChildren(); i++) {
			const JsonTree* pChild = folder->GetChild(i);

			//only look at files
			if (pChild->GetType() != JNT_FILE) {
				continue;
			}

			T* newBindable = new T();
			newBindable->LoadBindingsFromJson(pChild);
			outResult.Push(newBindable);
		}
	};
}