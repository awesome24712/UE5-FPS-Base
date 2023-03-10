#include "JsonTree.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "../FileIO/Logger.h"

FString JsonTree::s_noKeyDefinedWarning = "NO KEY DEFINED";
FString JsonTree::s_typeNotStringWarning = "TYPE NOT JNT_STRING";

JsonTree::JsonTree(JsonTree* pParent, int iNumChildren, EJsonNodeType type, FString key) {
	Log("Creating tree at address %p with key %s\n", this, TCHAR_TO_ANSI(*key));
	m_parent = pParent;
	m_iNumChildren = iNumChildren;
	m_eType = type;
	m_key = key;
	m_children = NULL;

	if (iNumChildren > 0) {
		m_children = new JsonTree * [iNumChildren];
		memset(m_children, 0, sizeof(JsonTree**) * iNumChildren);
	}
	//Log("Created %i children at %p, parent is at %p\n", iNumChildren, m_children, this);
	for (int i = 0; i < iNumChildren; i++) {
		//Log("\t child at %p", m_children[i]);
	}
}
JsonTree::~JsonTree() {
	//Log(__FUNCTION__ "\n");
	for (int i = 0; i < m_iNumChildren; i++) {
		delete m_children[i];
	}
	delete[] m_children;
}

//adds an existing tree to be a child of this tree.
void JsonTree::SetChild(int index, JsonTree* child) {
	//Log(__FUNCTION__);
	//sanity check
	if (!child || child->GetParent()
		|| index >= m_iNumChildren
		|| m_children[index] != NULL) {
		Log(" ERROR setting child at %p to index %i\n", child, index);
		Log("(%i, %p, %p)", m_iNumChildren, child->GetParent(), m_children[index]);
		NLogger::Fatal("Bad JsonTree loading in %s", __FUNCTION__);
	}
	else {
		m_children[index] = child;
		child->m_parent = this;
		//Log("Setting %ith child at %p, parent is at %p\n", index, child, this);
	}
}

void JsonTree::SetValue(const FString& value) {
	m_sValue = value;
}
void JsonTree::SetValue(bool value) {
	m_bValue = value;
}
void JsonTree::SetValue(double value) {
	m_dValue = value;
}

const JsonTree* JsonTree::GetChild(int index) const {
	if (m_iNumChildren == 0
		|| index < 0
		|| index >= m_iNumChildren) {
		return NULL;
	}
	return m_children[index];
}

const JsonTree* JsonTree::GetLastChild() const {
	if (m_iNumChildren == 0) return NULL;

	return m_children[m_iNumChildren - 1];
}

const JsonTree* JsonTree::GetChild(const FString& key) const {
	const JsonTree* result = NULL;
	for (int i = 0; i < m_iNumChildren; i++) {
		if (m_children[i]->Key() == key) {
			result = m_children[i];
			break;
		}
	}
	return result;
}


bool JsonTree::AllChildrenOfType(EJsonNodeType type) const {
	//base initial value on presence of children
	bool result = m_iNumChildren > 0;

	for (int i = 0; i < m_iNumChildren && result; i++) {
		result = GetChild(i)->GetType() == type;
	}

	return result;
}

FString JsonTree::ToString(int depth) const {
	//Log(__FUNCTION__ ", printing tree at %p with key %s\n", this, TCHAR_TO_ANSI(*Key()));

	const char* pszTab = "    ";

	//append the correct number of tabs
	FString result = "";
	for (int i = 0; i < depth; i++) {
		result = result + pszTab;
	}

	//append key if available
	if (HasKey()) {
		result = result + '"' + Key() + "\" : ";
	}

	//next depends on type
	if (m_eType == JNT_STRING) {
		result = result + '"' + GetValueString() + '"';
	}
	else if (m_eType == JNT_DOUBLE) {
		char buffer[32];
		sprintf_s(buffer, "%f", GetValueDouble());
		result = result + buffer;
	}
	else if (m_eType == JNT_BOOLEAN) {
		result = result + (m_bValue ? "true" : "false");
	}
	else if (m_eType == JNT_NONEXISTANT) {
		result = result + "null";
	}
	else if (m_eType == JNT_OBJECT) {
		result = result + "{\n";
		for (int i = 0; i < m_iNumChildren; i++) {
			result += GetChild(i)->ToString(depth + 1);
		}
		for (int i = 0; i < depth; i++) {
			result = result + pszTab;
		}
		result += '}';
	}
	else if (m_eType == JNT_ARRAY) {
		result = result + "[\n";
		for (int i = 0; i < m_iNumChildren; i++) {
			result += GetChild(i)->ToString(depth + 1);
		}
		for (int i = 0; i < depth; i++) {
			result = result + pszTab;
		}
		result += ']';
	}

	if (GetParent() 
		/* && GetParent()->GetLastChild() != this*/) {
		//Log("Print parent is at %p\n", GetParent());
		result += ',';
	}
	result += '\n';

	return result;
}

JsonTreeHandle::JsonTreeHandle(const JsonTree* root) {
	if (root) {
		//Log(__FUNCTION__ "\n");
		m_pTree = root;
		m_piReferenceCount = new int(1);
	}
	else {
		m_pTree = NULL;
		m_piReferenceCount = NULL;
	}
}