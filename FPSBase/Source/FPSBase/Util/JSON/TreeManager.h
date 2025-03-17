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

	void LoadBindablesFromFile(IJsonBindable* target, const FString& fileName);

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

	//type T must inherit from IJsonBindable
	//also, any pre-existing T* is freed from memory
	//similar to LoadBindablesFromFolder, but looks at direct children of the file
	template<class T>
	void LoadBindablesFromFile(TArray<T*>& outResult, const FString& fileName) {
		const JsonTree* file = FindFile(fileName);
		if (!file) {
			return;
		}

		//delete any pre-existing data
		for (int i = 0; i < outResult.Num(); i++) {
			delete outResult[i];
		}

		outResult = TArray<T*>();

		for (int i = 0; i < file->NumChildren(); i++) {
			const JsonTree* pChild = file->GetChild(i);

			//only look at files
			if (pChild->GetType() != JNT_OBJECT) {
				continue;
			}

			T* newBindable = new T();
			newBindable->LoadBindingsFromJson(pChild);
			outResult.Push(newBindable);
		}
	};
}