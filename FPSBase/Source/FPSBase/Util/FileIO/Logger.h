#pragma once
#include "../Predefs.h"

namespace NLogger {
	void OpenLogFile();

	void Log(const char* pszMsg);

	void CloseLogFile();

	void Message(const FString&);
	void Message(const char* pszFormat, ...);
	void Message(FColor c, ftime duration, const char* pszFormat, ...);

	void Blurp(const FString&);
	void Blurp(const char* pszFormat, ...);

	void Warning(const FString&);
	void Warning(const char* pszFormat, ...);

	void Fatal(const FString&);
	void Fatal(const char* pszFormat, ...);
}

inline const wchar_t*	WCStr(const FString& str) { return &(str[0]); }
//inline const char*		CStr(const FString& str) { return StringCast<char>(*str).Get(); }
#define					CStr(str) StringCast<char>(*str).Get()
void					Msg(const char* pszFormat, ...);
void					Log(const char* pszFormat, ...);
void					Log(const wchar_t* pszFormat, ...);
void					Msg(const wchar_t* pszFormat, ...);
inline void				Msg(const FString& str) { Msg(WCStr(str)); }

#define NAME_TO_ANSI(name) TCHAR_TO_ANSI(*(name.GetPlainNameString()))