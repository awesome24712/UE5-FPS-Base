#include "PropertyBinder.h"

using namespace NJsonPropertyBinding;

JTClassBindingSet::JTClassBindingSet(FName name, IJsonBindableFactoryBase* pFactoryOnHeap, UClass* pClass) {
	m_bindings = TArray<Binding>();
	m_pClass = pClass;
	m_name = name;
	m_pFactory = pFactoryOnHeap;
}

static TMap<FName, JTClassBindingSet*> g_classNameBindingSets;

static JTClassBindingSet* g_pCurrentBindingSet;

JTClassBindingSet* JTClassBindingSet::__CreateBindingSet(FName name, IJsonBindableFactoryBase* pFactoryOnHeap, UClass* pClass) {

	if (g_pCurrentBindingSet) {
		NLogger::Fatal("Creating new binding set but %s is still the active one", NAME_TO_ANSI(g_pCurrentBindingSet->m_name));
		return NULL;
	}

	//always create a new binding set in case old one is out of date
	g_pCurrentBindingSet = new JTClassBindingSet(name, pFactoryOnHeap, pClass);

	//if we are creating a set which already exists, delete and remove the old one!
	if (g_classNameBindingSets.Contains(name)) {
		auto oldSet = g_classNameBindingSets.FindAndRemoveChecked(name);

		//transfer the map first too so we don't forget them
		g_pCurrentBindingSet->m_factoryMap = oldSet->m_factoryMap;
		g_pCurrentBindingSet->m_factoryList = oldSet->m_factoryList;

		delete oldSet;
		Msg("Deleting old duplicate bindingset %s", NAME_TO_ANSI(name));
	}

	return g_classNameBindingSets.Add(name, g_pCurrentBindingSet);
}

void JTClassBindingSet::__CreateBinding(size_t offset, FName bindingName, EJsonNodeType type, bool bRequired) {
	//if we are creating a set which already exists, delete and remove the old one!
	if (g_pCurrentBindingSet) {
		JTClassBindingSet* bs = g_pCurrentBindingSet;
		//verify this binding does not already exist
		for (int i = 0; i < bs->m_bindings.Num(); i++) {
			if (bs->m_bindings[i].m_bindingName == bindingName) {
				NLogger::Fatal("Duplicate binding %s on class %s", NAME_TO_ANSI(bindingName), NAME_TO_ANSI(bs->m_name));
				return;
			}
		}

		//create the binding
		bs->m_bindings.Emplace(Binding{
			offset, type, bindingName, bRequired
		});
		Msg("Created binding for with bindingName %s", NAME_TO_ANSI(bindingName));
	}
	else {
		NLogger::Fatal("Failed to bind json field %s", NAME_TO_ANSI(bindingName));
	}
}

void JTClassBindingSet::__FinishBinding() {
	if (g_pCurrentBindingSet) {
		if (g_pCurrentBindingSet->m_bindings.Num() == 0) {
			NLogger::Warning("Finishing binding %s with 0 bindings, is this right?", NAME_TO_ANSI(g_pCurrentBindingSet->m_name));
		}
		g_pCurrentBindingSet = NULL;
	}
	else {
		NLogger::Fatal("In " __FUNCTION__ " attempting to end BindingSet which does not exist or has already ended.");
	}
}

void JTClassBindingSet::__FinishBinding(void (*postLoadCallback)(IJsonBindable*)) {
	if (g_pCurrentBindingSet) {
		g_pCurrentBindingSet->m_pPostLoadCallback = postLoadCallback;
	}
	__FinishBinding();
}

JTClassBindingSet* JTClassBindingSet::FindBindingSet(const FName& name) {
	JTClassBindingSet** pSet = g_classNameBindingSets.Find(name);
	if (pSet) {
		return *pSet;
	}
	else {
		return nullptr;
	}
}

void JTClassBindingSet::DeleteObjectsOfBindingSet(const FName& name) {
	JTClassBindingSet* pSet = FindBindingSet(name);

	if (!pSet) {
		NLogger::Fatal("Could not find binding set %s while attempting to delete objects of that type", NAME_TO_ANSI(name));
		return;
	}

	for (int i = 0; i < pSet->m_factoryList.Num(); i++) {
		IJsonBindable* pObj = pSet->m_factoryList[i];
		pSet->m_factoryList.RemoveAt(i);
		delete pObj; //it is removed from map in destructor, a bit awkward but technically more efficient
	}
}

void IJsonBindable::LoadBindingsFromJson(const JsonTree* pTree) {
	Msg(__FUNCTION__);

	if (!m_bindingSet) {
		NLogger::Fatal("Cannot load JsonTree with key %s because BindingSet is missing!", NAME_TO_ANSI(pTree->Key()));
		return;
	}

	/*
	* Always add ourselves to the list of objects created with this binding set, so we won't lose track
	* of POD types in the heap
	*/
	m_bindingSet->m_factoryList.Push(this);

	if (m_bindingSet->m_bindings.Num() == 0) {
		NLogger::Fatal("Cannot load JsonTree bindings because bindingSet %s has zero bindings defined", NAME_TO_ANSI(m_bindingSet->GetName()));
		return;
	}

	//bind to our codename if one is providing
	const JsonTree* pCodeNameTree = pTree->GetChild("codeName");
	if (pCodeNameTree && pCodeNameTree->GetType() == JNT_NAME) {
		m_codeName = pCodeNameTree->GetValueName();
	}
	else if (pCodeNameTree) {
		NLogger::Warning("JsonTree with key %s has invalid value type for codeName", NAME_TO_ANSI(pTree->Key()));
	}
	else if (pTree->HasKey()) {
		m_codeName = pTree->Key();
	}
	else {
		NLogger::Message("Missing both codeName and key for JsonTree, is this intentional?", NAME_TO_ANSI(pTree->Key()));
	}

	//Loop through all our bindings and load them from the tree
	for (int i = 0; i < m_bindingSet->m_bindings.Num(); i++) {
		Binding& b = m_bindingSet->m_bindings[i];

		unsigned char* pBoundLocation = m_pOwnerMemoryOrigin + b.m_offset;

		const JsonTree* pChildNode = pTree->GetChild(b.m_bindingName);

		//Check for missing child, a NULL child, or a child whose type doesn't match what we expect
		//also account for double to int conversion
		if (!pChildNode 
			|| pChildNode->GetType() == JNT_NONEXISTANT
			|| (pChildNode->GetType() != b.m_jnt 
				&& !(b.m_jnt == JNT_DOUBLE_TO_INT 
					&& pChildNode->GetType() == JNT_DOUBLE)
				&& !(b.m_jnt == JNT_NAME_ARRAY
					&& pChildNode->GetType() == JNT_ARRAY)
				)
			)
		{
			//report error for missing child that is required
			if (!pChildNode && b.m_bRequired) {
				NLogger::Fatal("Required field %s is missing from json!", NAME_TO_ANSI(b.m_bindingName));
			}
			else if (pChildNode && pChildNode->GetType() != b.m_jnt) {
				NLogger::Fatal("Unexpected value type for field %s in json!", NAME_TO_ANSI(b.m_bindingName));
			}
			continue;
		}

		IJsonBindable* pObject;
		TArray<IJsonBindable*>* pArray;
		TArray<FName>* pStringArray;
		FName* pString;
		int* pInt;
		double* pDouble;
		bool* pBool;
		float* pFloat;
		uint8* pByte;
		uint16* puShort;

		switch (b.m_jnt) {
		case JNT_OBJECT:
			pObject = reinterpret_cast<IJsonBindable*>(pBoundLocation);
			pObject->LoadBindingsFromJson(pChildNode);
			break;
		case JNT_NAME:
			pString = reinterpret_cast<FName*>(pBoundLocation);
			*pString = pChildNode->GetValueName();
			break;
		case JNT_ARRAY:
			pArray = reinterpret_cast<TArray<IJsonBindable*>*>(pBoundLocation);
			if (pChildNode->NumChildren() >= 2) {
				//get binding set name for object type from the first entry in the array, ensure it is string
				if (pChildNode->GetChild(0)->GetType() != JNT_NAME) {
					NLogger::Fatal("Object array %s missing binding set name as first entry!", NAME_TO_ANSI(b.m_bindingName));
					break;
				}
				JTClassBindingSet* bs = FindBindingSet(pChildNode->GetChild(0)->GetValueName());
				if (!bs) {
					NLogger::Fatal("Object array %s has unknown binding set %s!", NAME_TO_ANSI(b.m_bindingName), NAME_TO_ANSI(pChildNode->GetChild(0)->GetValueName()));
					break;
				}

				//index starts at one because the first entry was a string with the binding set name
				for (int j = 1; j < pChildNode->NumChildren(); j++) {
					if (pChildNode->GetChild(j)->GetType() != JNT_OBJECT) {
						NLogger::Warning("Unexpected non-object in object array %s, skipping!", NAME_TO_ANSI(b.m_bindingName));
						break;
					}
					IJsonBindable* pObj = bs->Create();
					pObj->LoadBindingsFromJson(pChildNode->GetChild(j));
					pArray->Push(pObj);
				}
			}
			else {
				NLogger::Warning("Skipping too-short object array %s!", NAME_TO_ANSI(b.m_bindingName));
			}

			break;
		case JNT_NAME_ARRAY:
			pStringArray = reinterpret_cast<TArray<FName>*>(pBoundLocation);
			for (int j = 0; j < pChildNode->NumChildren(); j++) {
				if (pChildNode->GetType() == JNT_NAME) {
					pStringArray->Push(pChildNode->GetValueName());
				}
				else {
					NLogger::Warning("Unexpected non-string in string array %s, skipping!", NAME_TO_ANSI(b.m_bindingName));
				}
			}
		case JNT_DOUBLE_TO_INT:
			pInt = reinterpret_cast<int*>(pBoundLocation);
			*pInt = (int)pChildNode->GetValueDouble();
		case JNT_DOUBLE_TO_FLOAT:
			pFloat = reinterpret_cast<float*>(pBoundLocation);
			*pFloat = (float)pChildNode->GetValueDouble();
		case JNT_DOUBLE_TO_BYTE:
			pByte = reinterpret_cast<uint8*>(pBoundLocation);
			*pByte = (uint8)pChildNode->GetValueDouble();
		case JNT_DOUBLE_TO_USHORT:
			puShort = reinterpret_cast<uint16*>(pBoundLocation);
			*puShort = (uint16)pChildNode->GetValueDouble();
		case JNT_DOUBLE:
			pDouble = reinterpret_cast<double*>(pBoundLocation);
			*pDouble = pChildNode->GetValueDouble();
			break;
		case JNT_BOOLEAN:
			pBool = reinterpret_cast<bool*>(pBoundLocation);
			*pBool = pChildNode->GetValueBool();
			break;
		}
	}

	//if we have a codename, add ourselves to the list of objects created with this binding set
	if (m_codeName.GetStringLength() > 0) {
		IJsonBindable** ppExisting = m_bindingSet->m_factoryMap.Find(m_codeName);
		if (ppExisting) {
			if (*ppExisting == this) {
				NLogger::Blurp("Binding pre-existing IJsonBindable with codeName %s", NAME_TO_ANSI(m_codeName));
			}
			else {
				NLogger::Warning("IJsonBindable of type %s found with duplicate codeName %s, skipping!", 
					NAME_TO_ANSI(m_bindingSet->m_name), NAME_TO_ANSI(m_codeName));
			}
		}
		else {
			m_bindingSet->m_factoryMap.Add(m_codeName, this);
		}
	}

	//everything else is done -- call post-load callback if we have one
	if (m_bindingSet->m_pPostLoadCallback) {
		m_bindingSet->m_pPostLoadCallback(this);
	}
}

void IJsonBindable::LoadBindingsFromJson(FString path) {
	JsonTreeHandle jth = JsonTreeHandle::CreateFromFile(path);

	if (jth) {
		LoadBindingsFromJson(jth.Get());
	}
	else {
		NLogger::Fatal("Could not LoadBindingsFromJson from path %s", CStr(path));
	}
}