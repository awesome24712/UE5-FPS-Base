#pragma once
#include "CoreMinimal.h"
#include "JsonTree.h"

namespace NJsonPropertyBinding {

	struct Binding {
		//offset from object pointer to member of that object, 
		//must be relative to pointer of the class in which those members are defined
		size_t m_offset; 

		//what type of value are we binding to?
		//JNT_FOLDER --> not applicable to bindings
		//JNT_FILE --> same as object?
		//JNT_OBJECT --> subobject which is also serializable
		//JNT_STRING --> FString
		//JNT_ARRAY --> TArray<T>
		//JNT_NUMBER --> double
		//JNT_BOOLEAN --> boolean
		//JNT_NONEXISTANT --> pointer or reference?
		EJsonNodeType m_jnt; 

		//what key to look for in the jsons we're given, the value corresponding to this key is used as
		//the data to send through the binding
		FString m_bindingName;

		bool m_bRequired; //whether or not to throw an error if the json excludes this binding
	};
}

class IJsonBindable;

class IJsonBindableFactoryBase {
public: virtual IJsonBindable* Create() = 0;
	  virtual ~IJsonBindableFactoryBase() {}
};

template<class J> 
class IJsonBindableFactory : public IJsonBindableFactoryBase {
public: 
	IJsonBindable* Create() override { return new J(); }
};

class JTClassBindingSet {
	friend class IJsonBindable;
private:
	TArray<NJsonPropertyBinding::Binding> m_bindings;
	bool	m_bFinishedCreatingBindings;
	UClass* m_pClass;
	FString m_name;
	IJsonBindableFactoryBase* m_pFactory;

	//objects belonging to this binding set are added and removed from this map as they are created/destroyed
	TMap<FString, IJsonBindable*> m_factoryMap; //every object with a codename
	TArray<IJsonBindable*> m_factoryList; //every object regardless of codename presence

	void (*m_pPostLoadCallback)(IJsonBindable*); //function to call on object after loading from JSON

	JTClassBindingSet(FString name, IJsonBindableFactoryBase* pFactoryOnHeap, UClass* pClass);
	~JTClassBindingSet() { delete m_pFactory; }
public:

	//Creates a binding for a class with a given name.
	//Name provided here doesn't strictly have to match class name
	//UClass is optional
	static JTClassBindingSet* __CreateBindingSet(FString name, IJsonBindableFactoryBase* pFactoryOnHeap, UClass* pClass = NULL);

	static void __CreateBinding(size_t offset, FString bindingName, EJsonNodeType type, bool bRequired);

	static void __FinishBinding();

	static void __FinishBinding(void (*postLoadCallback)(IJsonBindable*));

	FString GetName() const { return m_name; }

	static JTClassBindingSet* FindBindingSet(const FString& name);

	virtual IJsonBindable* Create() { return m_pFactory ? m_pFactory->Create() : nullptr; };

	const TArray<IJsonBindable*>& GetAll() { return m_factoryList; }
	const TMap<FString, IJsonBindable*>& GetAllNamed() { return m_factoryMap; }

	/*
	* Deletes objects created with this binding set from memory.
	* This should only be done for objects which do NOT exist in the game world.
	* I.e. do NOT call this for actors, etc.
	* Rather this should only be done for POD types where we're managing the lifecycle ourselves
	*/
	static void DeleteObjectsOfBindingSet(const FString& name);

	const TArray<NJsonPropertyBinding::Binding>& GetBindings() const { return m_bindings; }
};

inline JTClassBindingSet* FindBindingSet(const FString& name) { return JTClassBindingSet::FindBindingSet(name); }


class IJsonBindable {
	friend class JTClassBindingSet;
protected:
	JTClassBindingSet* m_bindingSet;
	unsigned char* m_pOwnerMemoryOrigin;
	FString m_codeName = L"";

public:
	~IJsonBindable() {
		//find and remove ourselves from the map of objects of this binding set
		if (m_codeName.Len() > 0 && m_bindingSet) {
			if (m_bindingSet->m_factoryMap.Contains(m_codeName)) {
				m_bindingSet->m_factoryMap.Remove(m_codeName);
			}
		}
		if (!m_bindingSet) {
			NLogger::Warning("Missing binding set!");
		}
	}

	void LinkObjectToBindingSet(JTClassBindingSet* bs, unsigned char* self) {
		m_bindingSet = bs;
		m_pOwnerMemoryOrigin = self;
	}

	void LoadBindingsFromJson(const JsonTree* pTree);
	void LoadBindingsFromJson(FString path);

	inline FString GetCodeName() const { return m_codeName; }

	const TMap<FString, IJsonBindable*>& GetAllNamedOfThisType() const { return m_bindingSet->m_factoryMap; };
	const TArray<IJsonBindable*>& GetAllOfThisType() const { return m_bindingSet->m_factoryList; };

	JsonTreeHandle ToJsonTree();
private:
	void ToJsonTree(JsonTree* jt); //recursive implementation function for above function
};

#ifndef JT_CALC_MEMORY_OFFSET
#define JT_CALC_MEMORY_OFFSET(member) ((unsigned char*)(&member) - (unsigned char*)this)
#define JT_CALC_MEMORY_OFFSET_OBJECT(member) ((unsigned char*)(static_cast<IJsonBindable*>(&member)) - (unsigned char*)this)

#define JT_START_BINDING_UCLASS(name, _class) LinkObjectToBindingSet(JTClassBindingSet::__CreateBindingSet(name, nullptr, GetClass()), reinterpret_cast<unsigned char*>(this))
#define JT_START_BINDING(name, _class) LinkObjectToBindingSet(JTClassBindingSet::__CreateBindingSet(name, new IJsonBindableFactory<_class>()), reinterpret_cast<unsigned char*>(this))

//#define JT_BIND_OBJECT(member, bindingName) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_OBJECT, true)
#define JT_BIND_OBJECT(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET_OBJECT(member), bindingName, JNT_OBJECT, required)

//#define JT_BIND_STRING(member, bindingName) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_STRING, true)
#define JT_BIND_STRING(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_STRING, required)

#define JT_BIND_COLOR(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_STRING_TO_COLOR, required)

//#define JT_BIND_ARRAY(member, bindingName) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_ARRAY, true)
#define JT_BIND_ARRAY(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_ARRAY, required)

#define JT_BIND_STRING_ARRAY(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_STRING_ARRAY, required)

//#define JT_BIND_DOUBLE(member, bindingName) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_NUMBER, true)
#define JT_BIND_DOUBLE(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_DOUBLE, required)

#define JT_BIND_INT(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_DOUBLE_TO_INT, required)

#define JT_BIND_FLOAT(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_DOUBLE_TO_FLOAT, required)

#define JT_BIND_BYTE(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_DOUBLE_TO_BYTE, required)

#define JT_BIND_USHORT(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_DOUBLE_TO_USHORT, required)

//#define JT_BIND_BOOLEAN(member, bindingName) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_BOOLEAN, true)
#define JT_BIND_BOOLEAN(member, bindingName, required) JTClassBindingSet::__CreateBinding(JT_CALC_MEMORY_OFFSET(member), bindingName, JNT_BOOLEAN, required)

#define JT_FINISH_BINDING() JTClassBindingSet::__FinishBinding();
#define JT_FINISH_BINDING_WITH_CALLBACK(func) JTClassBindingSet::__FinishBinding(func);

#endif