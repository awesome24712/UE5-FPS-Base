#include "Tokenizer.h"
#include "Logger.h"
//#include "CoreMisc.h"

namespace NTokenizer {

	void TokenizeFile(FString pathRelativeToMods, TArray<Token>& result) {
		Log(__FUNCTION__ "\n");

		FString newPath = FPaths::ProjectModsDir() + pathRelativeToMods;
		FString fileText;

		if (!FFileHelper::LoadFileToString(fileText, &newPath[0])) {
			NLogger::Fatal("Could not open %s for JSON tokenization", TCHAR_TO_ANSI(*pathRelativeToMods));
			return;
		}

		//Tokenize one character at a time
		result = TArray<Token>();
		int lineNumber = 1;
		FString whiteSpace = " \t\r";

		bool bInComment = false;
		for (int i = 0; i < fileText.Len(); i++) {
			TCHAR c = fileText[i];

			//check for newline
			if (c == '\n') {
				lineNumber++;
				bInComment = false;
				continue;
			}

			if (bInComment) continue;
		
			//first check against whitespace, skip
			bool bSkipEmplace = false;
			for (int j = 0; j < whiteSpace.Len(); j++) {
				if (c == whiteSpace[j]) {
					bSkipEmplace = true;
					break;
				}
			}

			//also check for comments
			if (!bSkipEmplace && c == '/' && i < (fileText.Len() - 1) && fileText[i + 1] == '/') {
				bSkipEmplace = true;
				i++; //extra incrament
				bInComment = true; //ignore future items until newline
			}

			if (!bSkipEmplace) {
				//okay now build token
				Token t = { lineNumber, c };
				result.Emplace(t);
			}
			
		}
	}

}