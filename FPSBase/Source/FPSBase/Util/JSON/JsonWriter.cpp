#include "CoreMinimal.h"
#include "../../Util.h"

JsonTreeHandle IJsonBindable::ToJsonTree() {
	JsonTreeHandle jth = JsonTreeHandle::CreateEmptyForEditing();
	JsonTree* jt = jth.GetForEditing();
	ToJsonTree(jt);
	return jth;
}

void IJsonBindable::ToJsonTree(JsonTree* jt) {
	jt->SetType(JNT_OBJECT);
	JTClassBindingSet& bs = *m_bindingSet;

	bool bHasCodeName = !!m_codeName.Len();
	jt->ReserveChildren(bs.GetBindings().Num() + bHasCodeName);

	if (bHasCodeName) {
		auto cn = jt->AddChild("codeName");
		cn->SetValue(m_codeName);
	}

	for (auto b : bs.GetBindings()) {
		auto c = jt->AddChild(b.m_bindingName, b.m_jnt);
		c->SetType(b.m_jnt);

		unsigned char* pBoundLocation = m_pOwnerMemoryOrigin + b.m_offset;

		IJsonBindable* pObject;
		TArray<IJsonBindable*>* pArray;
		TArray<FString>* pStringArray;
		FString* pString;
		int* pInt;
		double* pDouble;
		bool* pBool;
		float* pFloat;
		uint8* pByte;
		uint16* puShort;
		FColor* pColor;
		switch (b.m_jnt) {
		case JNT_OBJECT:
			pObject = reinterpret_cast<IJsonBindable*>(pBoundLocation);
			c->SetType(JNT_OBJECT);
			pObject->ToJsonTree(c);
			break;
		case JNT_STRING:
			pString = reinterpret_cast<FString*>(pBoundLocation);
			c->SetValue(*pString);
			break;
		case JNT_STRING_TO_COLOR:
			pColor = reinterpret_cast<FColor*>(pBoundLocation);
			c->SetValue(ColorToString(*pColor));
			break;
		case JNT_ARRAY:
			pArray = reinterpret_cast<TArray<IJsonBindable*>*>(pBoundLocation);
			
			//skip if no children
			if (pArray->Num() == 0) continue;

			c->ReserveChildren(pArray->Num() + 1); //+1 is for binding set name
			c->AddChild("")->SetValue((*pArray)[0]->m_bindingSet->GetName());

			for (int i = 0; i < pArray->Num(); i++) {
				(*pArray)[i]->ToJsonTree(c->AddChild("", JNT_OBJECT));
			}

			break;
		case JNT_STRING_ARRAY:
			pStringArray = reinterpret_cast<TArray<FString>*>(pBoundLocation);
			//skip if no children
			if (pStringArray->Num() == 0) continue;

			c->ReserveChildren(pStringArray->Num());

			for (int i = 0; i < pStringArray->Num(); i++) {
				c->AddChild("")->SetValue((*pStringArray)[i]);
			}

			break;
		case JNT_DOUBLE_TO_INT:
			pInt = reinterpret_cast<int*>(pBoundLocation);
			c->SetValue((double)*pInt);
			break;
		case JNT_DOUBLE_TO_FLOAT:
			pFloat = reinterpret_cast<float*>(pBoundLocation);
			c->SetValue((double)*pFloat);
			break;
		case JNT_DOUBLE_TO_BYTE:
			pByte = reinterpret_cast<uint8*>(pBoundLocation);
			c->SetValue((double)*pByte);
			break;
		case JNT_DOUBLE_TO_USHORT:
			puShort = reinterpret_cast<uint16*>(pBoundLocation);
			c->SetValue((double)*puShort);
			break;
		case JNT_DOUBLE:
			pDouble = reinterpret_cast<double*>(pBoundLocation);
			c->SetValue(*pDouble);
			break;
		case JNT_BOOLEAN:
			pBool = reinterpret_cast<bool*>(pBoundLocation);
			c->SetValue(*pBool);
			break;
		default:
			NLogger::Fatal(__FUNCTION__ " Unknown JNT %i\n", b.m_jnt);
			return;
		}
	}
}