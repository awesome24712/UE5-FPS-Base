#pragma once
#include "CoreMinimal.h"
#include "InputCoreTypes.h"

namespace NTokenizer {

	struct Token {
		int m_iLineNumber;
		TCHAR m_char;
	};

	void TokenizeFile(const FString& path, TArray<Token>& result);



}