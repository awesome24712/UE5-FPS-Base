#pragma once
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "../FileIO/Tokenizer.h"
#include "../FileIO/Logger.h"
//#include "Engine/EngineTypes.h"
//#include "Engine/EngineBaseTypes.h"


//These lines below are just for letting our parser access JsonTree constructor
class JsonTree;
namespace NJsonParser {
	JsonTree* ParseLine(bool bParsingObject);
	JsonTree* ParseObject(FString key);
	JsonTree* ParseArray(FString key);
}

enum EJsonNodeType {
	JNT_FOLDER, //name of folder containing jsons, used for determining json-to-class mappings
	JNT_FILE, //an top-level json file, the name of the file (excluding extension) is the key
	JNT_OBJECT,
	JNT_STRING,
	JNT_ARRAY,
	JNT_DOUBLE,
	JNT_BOOLEAN,
	JNT_NONEXISTANT, //either is set to null or doesn't exist as a key

	JNT_DOUBLE_TO_INT, //used only by bindings to indicate that a double from the JSON should be converted to an int in memory
	JNT_DOUBLE_TO_FLOAT, //used only by bindings to indicate that a double from the JSON should be converted to a float in memory
	JNT_DOUBLE_TO_BYTE, //used only by bindings to indicate that a double from the JSON should be converted to a byte in memory
	JNT_STRING_ARRAY //used only by bindings to indicate that an array from JSON should be converted to TArray<String>
};

//Handler helps with memory management and creating JsonTree's
class JsonTreeHandle;

class JsonTree {
private:
	friend class JsonTreeHandle;
	friend JsonTree* NJsonParser::ParseLine(bool bParsingObject);
	friend JsonTree* NJsonParser::ParseObject(FString key);
	friend JsonTree* NJsonParser::ParseArray(FString key);

	//static JsonTree* CreateFromPath(FString path);

	JsonTree(JsonTree* pParent, int iNumChildren, EJsonNodeType type, FString key);
	FORCENOINLINE ~JsonTree();
	void SetChild(int index, JsonTree* child);
	

	JsonTree* m_parent;
	JsonTree** m_children;
	int m_iNumChildren;
	
	FString m_key;

	EJsonNodeType m_eType;

	//possible value types
	FString m_sValue;
	double m_dValue;
	bool m_bValue;

	static FString s_noKeyDefinedWarning;
	static FString s_typeNotStringWarning;

public:
	const JsonTree* GetParent() const { return m_parent; };
	int				NumChildren() const { return m_iNumChildren; };
	const JsonTree* GetChild(int index) const;
	const JsonTree* GetLastChild() const;
	const JsonTree* GetChild(const FString& key) const;
	bool			AllChildrenOfType(EJsonNodeType type) const;

	//Functions for modifying JsonTrees from elsewhere
	void			ReserveChildren(int numChildren); //reserves slots of more efficiently adding children. All slots must be filled
	JsonTree*		AddChild(const FString& key, EJsonNodeType type = JNT_STRING);
	void			SetValue(const FString& value);
	void			SetValue(bool value);
	void			SetValue(double value);
	void			SetKey(const FString& key) { m_key = key; }
	void			SetType(EJsonNodeType jnt) { m_eType = jnt; }


	bool HasKey() const { return m_key.Len() > 0; }; //Elements within arrays won't have keys
	const FString& Key() const {
		if (HasKey())
			return m_key;
		else
			return s_noKeyDefinedWarning;
	}

	EJsonNodeType GetType() const { return m_eType; }

	/*
	* Getters for accessing value as different types
	*/
	const FString& GetValueString() const {
		if (m_eType == JNT_STRING)
			return m_sValue;
		else
			return s_typeNotStringWarning;
	}

	double GetValueDouble() const {
		if (m_eType == JNT_DOUBLE)
			return m_dValue;
		else
			return 0;
	}

	bool GetValueBool() const {
		if (m_eType == JNT_STRING)
			return m_bValue;
		else
			return false;
	}

	FString ToString(int depth = 0) const;
};

class JsonTreeHandle {
public:
	JsonTreeHandle() {
		m_pTree = NULL;
		m_piReferenceCount = NULL;
	};

private:

	JsonTreeHandle(JsonTree* root);

	int* m_piReferenceCount;
	JsonTree* m_pTree;

	FORCENOINLINE void nullify() {
		(*m_piReferenceCount)--;
		if (*m_piReferenceCount == 0) {
			delete m_pTree;
			delete m_piReferenceCount;
		}
		m_piReferenceCount = NULL;
		m_pTree = NULL;
	}

	//This is called per-file to generate an entire file's JsonTree
	static JsonTree* CreateParentlessTreeFromTokens(TArray<NTokenizer::Token>& );

	//This is recursively called 
	static JsonTree* CreateParentlessTreeFromPath(const FString& path);
public:

	static JsonTreeHandle CreateEmptyForEditing();

	static JsonTreeHandle CreateFromDirectory(const FString& path);
	static JsonTreeHandle CreateFromFile(const FString& path);
	

	JsonTreeHandle(const JsonTreeHandle& otherPtr) {
		m_pTree = otherPtr.m_pTree;
		m_piReferenceCount = otherPtr.m_piReferenceCount;
		if (m_piReferenceCount)
			(*m_piReferenceCount)++;
	}
	JsonTreeHandle& operator=(const JsonTreeHandle& otherPtr) {
		if (this != &otherPtr) {
			if (m_pTree) {
				this->nullify();
			}
			m_pTree = otherPtr.m_pTree;
			m_piReferenceCount = otherPtr.m_piReferenceCount;
			if (m_piReferenceCount)
				(*m_piReferenceCount)++;
		}
		return *this;
	}

	const JsonTree* Get() const { return m_pTree; }
	JsonTree* GetForEditing() const { return m_pTree; }

	const JsonTree* operator ->() const {
		return m_pTree;
	}

	FORCENOINLINE ~JsonTreeHandle() {
		if (m_pTree) {
			nullify();
		}
		//NLogger::Fatal("MEMORY LEAK FIXME");
	}

	operator const JsonTree* () const { return m_pTree; }
	const JsonTree& operator*() const { return *m_pTree; }
	operator bool() const { return m_pTree != nullptr; }

	bool operator ==(const JsonTreeHandle& other) const {
		return this->m_pTree == other.m_pTree;
	}
};