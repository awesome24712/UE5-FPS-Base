#include "JsonTree.h"
#include "../FileIO/Logger.h"

//Generic c++ file and directory tools
#include <string>
#include <iostream>
#include <filesystem>

using namespace NTokenizer;

namespace NJsonParser {

	struct JsonParserException {
		FString m_message;
	};

	static TArray<Token> __emptyTokenList; //this just exists to make the compiler happy with my notation
	static TArray<Token>& tks = __emptyTokenList; //I don't normally like the lack of notation, but we're limiting ourselves to this file
	static FString g_currentParserPath;


	void VerifyNonEmpty() {
		if (tks.IsEmpty()) throw JsonParserException({ "Unexpected end of file. Did you forget to close an array, string, or object?" });
	}

	FORCENOINLINE void VerifyToken(TCHAR c) {
		if (!tks.IsEmpty() && tks[0].m_char == c) {
			//Log("%c\n", tks[0].m_char);
			tks.RemoveAt(0);
		}
		else if (!tks.IsEmpty()) {
			char buffer[32];
			_itoa_s(tks[0].m_iLineNumber, buffer, 10);
			throw JsonParserException{ 
				FString("Expected ") + c + " at line " + buffer + " but saw " + tks[0].m_char
			};
		}
		else {
			VerifyNonEmpty();
		}
	}

	//Verifies a token without consuming it
	void VerifyTokenPeek(TCHAR c) {
		if (tks.IsEmpty() || tks[0].m_char != c) {
			char buffer[32];
			_itoa_s(tks[0].m_iLineNumber, buffer, 10);
			throw JsonParserException{
				FString("Expected ") + c + " near line " + buffer + " when parsing " + g_currentParserPath
			};
		}
	}


	void ParseNull() {
		VerifyToken('n');
		VerifyToken('u');
		VerifyToken('l');
		VerifyToken('l');
	}

	bool ParseBoolean() {
		if (tks[0].m_char == 't') {
			VerifyToken('t');
			VerifyToken('r');
			VerifyToken('u');
			VerifyToken('e');
			return true;
		}
		else {
			VerifyToken('f');
			VerifyToken('a');
			VerifyToken('l');
			VerifyToken('s');
			VerifyToken('e');
			return false;
		}
	}

	//assumes next token is the first number
	double ParseNumber() {
		VerifyNonEmpty();

		//double result = 0.0;

		//verify format and build string, let someone else convert it into a double
		FString numberString = "";
		bool bFoundDecimalPoint = false;
		bool bFoundExponent = false;
		while (!tks.IsEmpty() && tks[0].m_char != ',' && tks[0].m_char != ']' && tks[0].m_char != '}') {
			TCHAR c = tks[0].m_char;

			if (c == '.') {
				if (bFoundDecimalPoint || bFoundExponent) {
					char buffer[32]; _itoa_s(tks[0].m_iLineNumber, buffer, 10);
					throw JsonParserException({ FString("Found badly formatted number on line ") + buffer + ", unexpected decimal point"});
				}
				bFoundDecimalPoint = true;
			}
			else if (c == 'e') {
				if (bFoundExponent || numberString.EndsWith(".")) {
					char buffer[32]; _itoa_s(tks[0].m_iLineNumber, buffer, 10);
					throw JsonParserException({ FString("Found badly formatted number on line ") + buffer + ", unexpected exponent marker e"});
				}
			} else if (!CharIsNumber(c)) {
				char buffer[32]; _itoa_s(tks[0].m_iLineNumber, buffer, 10);
				throw JsonParserException({ FString("Found badly formatted number on line ") + buffer + "unexpected token " + c});
			}

			numberString += c;
			tks.RemoveAt(0);
		}

		if (!CharIsNumber(numberString[numberString.Len() - 1])) {
			char buffer[32]; _itoa_s(tks[0].m_iLineNumber, buffer, 10);
			throw JsonParserException({ FString("Found badly formatted number on line ") + buffer + ", unexpected character " + numberString[numberString.Len() - 1] });
		}

		return FCString::Atod(*numberString);
	}

	//assumes next token is quote character
	FString ParseString() {
		VerifyNonEmpty();

		FString result = "";
		VerifyToken('"');

		//skip over $ required field indicator
		if (!tks.IsEmpty() && tks[0].m_char == '$') {
			tks.RemoveAt(0);
		}

		bool escapeSequence = false;
		while (!tks.IsEmpty() && tks[0].m_char != '"' && !escapeSequence) {
			if (tks[0].m_char == '\\') {
				escapeSequence = true;
			}
			result += tks[0].m_char;
			tks.RemoveAt(0);
		}

		VerifyToken('"');
		return result;
	}

	//assumes first token is " of the key, does NOT consume commas
	//when parsing an object, make sure we have keys
	JsonTree* ParseLine(bool bParsingObject) {
		VerifyNonEmpty();

		JsonTree* result = NULL;

		//consume a key if we have one. We might be in an array where keys aren't required
		//also don't confuse a string within an array as a key
		FString key = "";
		if (bParsingObject && tks[0].m_char == '"') {
			key = ParseString();
			VerifyToken(':');
		}
		else if (bParsingObject) {
			Msg("Throwing exception, next token is %c\n", tks[0].m_char);
			throw JsonParserException({"Members of objects must have keys"});
		}
		//Msg("Parsing line with key %s\n", TCHAR_TO_ANSI(*key));

		//determine what the value is based on next character
		TCHAR c = tks[0].m_char;
		if (c == 't' || c == 'f') {
			result = new JsonTree(NULL, 0, JNT_BOOLEAN, key);
			result->SetValue(ParseBoolean());
		}
		else if (c == 'n') {
			result = new JsonTree(NULL, 0, JNT_NONEXISTANT, key);
			VerifyToken('n'); VerifyToken('u'); VerifyToken('l'); VerifyToken('l');
		}
		else if (c == '{') {
			result = ParseObject(key);
		}
		else if (c == '[') {
			result = ParseArray(key);
		}
		else if (c == '"') {
			FString s = ParseString();
			result = new JsonTree(NULL, 0, JNT_STRING, key);
			result->SetValue(s);
		}
		else if (CharIsNumber(c)) {
			double d = ParseNumber();
			result = new JsonTree(NULL, 0, JNT_DOUBLE, key);
			result->SetValue(d);
		}
		else if (!tks.IsEmpty()) {
			char buffer[32]; _itoa_s(tks[0].m_iLineNumber, buffer, 10);
			Log("Unexpected character %c on line %i\n", tks[0].m_iLineNumber);
			Msg("Unexpected character %c on line %i\n", tks[0].m_iLineNumber);
			result = NULL;
			//throw JsonParserException({ FString("Unexpected character ") + tks[0].m_char + " on line " + buffer});
		}

		return result;
	}

	//assumes next token is the opening curly bracket
	//or the first " of this object's key
	JsonTree* ParseObject(FString key) {
		VerifyNonEmpty();
		//Log("Parsing object with key %s\n", TCHAR_TO_ANSI(*key));
		
		TArray<JsonTree*> children;

		VerifyToken('{');
		//parse one line individually first to set up our comma consumption
		if (!tks.IsEmpty() && tks[0].m_char != '}') {
			JsonTree* child = ParseLine(true);
			if (child) children.Push(child); else return NULL;
		}

		while (!tks.IsEmpty() && tks[0].m_char != '}') {
			VerifyToken(',');

			//extra check again, to be friendly to modders who accidentally leave an extra comma
			if (tks.IsEmpty() || tks[0].m_char == '}') {
				break;
			}

			JsonTree* child = ParseLine(true);
			if (child) children.Push(child); else return NULL;
		}
		VerifyToken('}');

		//build our own and put out children in
		JsonTree* pResult = new JsonTree(NULL, children.Num(), JNT_OBJECT, key);
		//Log("In " __FUNCTION__ " discovered %i children from tokens\n", children.Num());

		for (int i = 0; i < children.Num(); i++) {
			pResult->SetChild(i, children[i]);
		}

		//Log("Returning tree at %p with key %s\n", pResult, TCHAR_TO_ANSI(*pResult->Key()));
		return pResult;
	}

	//assumes next token is the opening square bracket
	JsonTree* ParseArray(FString key) {
		VerifyNonEmpty();
		TArray<JsonTree*> children;

		VerifyToken('[');
		//parse one line individually first to set up our comma consumption
		if (!tks.IsEmpty() && tks[0].m_char != ']') {
			JsonTree* child = ParseLine(false);
			if (child) children.Push(child); else return NULL;
		}

		while (!tks.IsEmpty() && tks[0].m_char != ']') {
			VerifyToken(',');

			//extra check again, to be friendly to modders who accidentally leave an extra comma
			if (tks.IsEmpty() || tks[0].m_char == ']') {
				break;
			}

			JsonTree* child = ParseLine(false);
			if (child) children.Push(child); else return NULL;
		}
		VerifyToken(']');

		//build our own and put out children in
		JsonTree* pResult = new JsonTree(NULL, children.Num(), JNT_ARRAY, key);

		for (int i = 0; i < children.Num(); i++) {
			pResult->SetChild(i, children[i]);
		}

		return pResult;
	}
}

using namespace NJsonParser;

JsonTree* JsonTreeHandle::CreateParentlessTreeFromTokens(TArray<Token>& tokens) {
	//Log(__FUNCTION__ "\n");

	tks = tokens;
	
	JsonTree* pResult = nullptr;

	try {
		pResult = ParseObject("");
	}
	catch (JsonParserException e) {
		NLogger::Fatal(e.m_message);
		NLogger::Fatal("This also causes a memory leak.");
	}

	return pResult;
}

JsonTreeHandle JsonTreeHandle::CreateFromFile(const FString& path) {
	//Log(__FUNCTION__ "\n");
	TArray<NTokenizer::Token> tokens;
	NTokenizer::TokenizeFile(path, tokens);

	if (tokens.Num() == 0) {
		NLogger::Warning("Failed to parse empty or missing JSON at %s", TCHAR_TO_ANSI(*path));
		return JsonTreeHandle(NULL);
	}

	return JsonTreeHandle(CreateParentlessTreeFromTokens(tokens));
}

JsonTree* JsonTreeHandle::CreateParentlessTreeFromPath(const FString& path) {
	namespace fs = std::filesystem;

	TArray<JsonTree*> foundChildren;

	for (const auto& entry : fs::directory_iterator(WCStr(path))) {
		JsonTree* child = nullptr;

		bool isDirectory = entry.is_directory();
		if (isDirectory) {
			child = CreateParentlessTreeFromPath(path + "/" + entry.path().filename().string().c_str());
		}
		else {
			FString extension = entry.path().extension().string().c_str();
			if (extension == FString(".json")) {
				TArray<NTokenizer::Token> tokens;
				NTokenizer::TokenizeFile(path + "/" + entry.path().filename().string().c_str(), tokens);

				if (tokens.Num() == 0) {
					NLogger::Warning("Failed to parse empty or missing JSON at %s", TCHAR_TO_ANSI(*path));
					continue;
				}

				child = CreateParentlessTreeFromTokens(tokens);
			}
		}

		//if this file/path yielded a child, add it to our list of children
		if (child) {
			child->SetKey(entry.path().filename().string().c_str());
			foundChildren.Add(child);
		}


		//Msg("%i, %s, %s", (int)entry.is_directory(), entry.path().filename().string().c_str(), entry.path().extension().string().c_str());
	}

	//now that we have all the info we need about child files and directories, build our parent level node
	JsonTree* parentNode = nullptr;
	if (foundChildren.Num() > 0) {
		parentNode = new JsonTree(nullptr, foundChildren.Num(), JNT_FOLDER, path);
		for (int i = 0; i < foundChildren.Num(); i++) {
			parentNode->SetChild(i, foundChildren[i]);
		}
	}


	return parentNode;
}

JsonTreeHandle JsonTreeHandle::CreateFromDirectory(const FString& pathRelativeToMods) {
	
	return JsonTreeHandle(CreateParentlessTreeFromPath(FPaths::ProjectModsDir() + pathRelativeToMods));
		
}