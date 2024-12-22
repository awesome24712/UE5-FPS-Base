#include "JsonTree.h"
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "../FileIO/Logger.h"

FString JsonTree::s_noKeyDefinedWarning = "NO KEY DEFINED";
FString JsonTree::s_typeNotStringWarning = "TYPE NOT JNT_STRING";

FString			NJsonParser::g_jsonSearchKey;
EJsonNodeType	NJsonParser::g_jsonSearchType;


JsonTree::JsonTree(JsonTree* pParent, int iNumChildren, EJsonNodeType type, FString key) {
	//Log("Creating tree at address %p with key %s\n", this, TCHAR_TO_ANSI(*key));
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

void JsonTree::ReserveChildren(int numChildren) {
	if (m_eType != JNT_ARRAY
	&& m_eType != JNT_STRING_ARRAY
	&& m_eType != JNT_OBJECT) {
		NLogger::Fatal("JsonTree::ReserveChildren call FAILED, node %s is not of parent type", WCStr(m_key));
	}

	if (numChildren < m_iNumChildren) {
		NLogger::Fatal("JsonTree::ReserveChildren call FAILED, %i < %i", numChildren, m_iNumChildren);
		return;
	}

	if (numChildren == m_iNumChildren) {
		return;
	}

	JsonTree** newChildren = new JsonTree*[numChildren];
	//copy in old children
	for (int i = 0; i < m_iNumChildren; i++) {
		newChildren[i] = m_children[i];
	}

	//free memory
	delete[] m_children;
	m_children = newChildren;

	//ensure we remember our new number of children
	m_iNumChildren = numChildren;
}

JsonTree* JsonTree::AddChild(const FString& key, EJsonNodeType type) {
	//First let's create the new child
	JsonTree* newChild = new JsonTree(this, 0, type, key);

	//find an empty null slot in our list of children
	int emptySlot = -1;
	for (int i = 0; i < m_iNumChildren; i++) {
		if (m_children[i] == nullptr) {
			emptySlot = i;
			break;
		}
	}

	//if we didn't find an empty slot, create one
	//the function call also increment m_iNumChildren
	if (emptySlot == -1) {
		this->ReserveChildren(m_iNumChildren + 1);
		emptySlot = m_iNumChildren - 1;
	}
	
	//actually assign the child to the empty slot
	m_children[emptySlot] = newChild;

	return newChild;
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

FString JsonTree::ToString(bool bHideFilenameForWriting, int depth) const {
	//Log(__FUNCTION__ ", printing tree at %p with key %s\n", this, TCHAR_TO_ANSI(*Key()));

	const char* pszTab = "    ";

	//append the correct number of tabs
	FString result = "";
	for (int i = 0; i < depth; i++) {
		result = result + pszTab;
	}

	//append key if available
	//but sometimes hide it for files
	if (HasKey()
		&& !(bHideFilenameForWriting && GetType() == JNT_FILE)) {
		result = result + '"' + Key() + "\" : ";
	}

	char buffer[32];

	//next depends on type
	if (m_eType == JNT_STRING) {
		result = result + '"' + GetValueString() + '"';
	}
	else if (m_eType == JNT_DOUBLE || m_eType == JNT_DOUBLE_TO_FLOAT) {
		sprintf_s(buffer, "%f", GetValueDouble());
		result = result + buffer;
	}
	else if (m_eType == JNT_DOUBLE_TO_INT) {
		sprintf_s(buffer, "%i", (int)GetValueDouble());
		result = result + buffer;
	}
	else if (m_eType == JNT_DOUBLE_TO_BYTE) {
		sprintf_s(buffer, "%i", (int)((uint8)GetValueDouble()));
		result = result + buffer;
	}
	else if (m_eType == JNT_DOUBLE_TO_USHORT) {
		sprintf_s(buffer, "%i", (int)((uint16)GetValueDouble()));
		result = result + buffer;
	}
	else if (m_eType == JNT_BOOLEAN) {
		result = result + (m_bValue ? "true" : "false");
	}
	else if (m_eType == JNT_NONEXISTANT) {
		result = result + "null";
	}
	else if (m_eType == JNT_OBJECT || m_eType == JNT_FOLDER || m_eType == JNT_FILE) {
		result = result + "{\n";
		for (int i = 0; i < m_iNumChildren; i++) {
			result += GetChild(i)->ToString(bHideFilenameForWriting, depth + 1);
		}
		for (int i = 0; i < depth; i++) {
			result = result + pszTab;
		}
		result += '}';
	}
	else if (m_eType == JNT_ARRAY) {
		result = result + "[\n";
		for (int i = 0; i < m_iNumChildren; i++) {
			result += GetChild(i)->ToString(bHideFilenameForWriting, depth + 1);
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

const JsonTree* JsonTree::DFS(bool(*pPredicate)(const JsonTree*)) const {
	//test ourselves first
	if ((*pPredicate)(this)) {
		return this;
	}

	//ask our children to test themselves
	for (int i = 0; i < NumChildren(); i++) {
		auto possibleResult = GetChild(i)->DFS(pPredicate);
		if (possibleResult) {
			return possibleResult;
		}
	}

	//nothing found, return null
	return nullptr;
}

const JsonTree* JsonTree::BFS(bool(*pPredicate)(const JsonTree*)) const {
	//Queue of nodes yet to be tested
	TQueue<const JsonTree*> queue;

	//add the top level tree node to the queue
	queue.Enqueue(this);
	
	//keep testing for so long as there are trees in the queue
	while (!queue.IsEmpty()) {
		const JsonTree* next;
		queue.Dequeue(next);

		//test it
		if ((*pPredicate)(next)) {
			return next;
		}

		//nothing found, queue in the children
		for (int i = 0; i < next->NumChildren(); i++) {
			queue.Enqueue(next->GetChild(i));
		}
	}

	//nothing found, return null
	return nullptr;
}

JsonTreeHandle::JsonTreeHandle(JsonTree* root) {
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

JsonTreeHandle JsonTreeHandle::CreateEmptyForEditing() {
	JsonTree* empty = new JsonTree(NULL, 0, JNT_OBJECT, "");

	return JsonTreeHandle(empty);
}