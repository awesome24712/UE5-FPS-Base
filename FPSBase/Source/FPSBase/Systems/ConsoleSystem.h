#pragma once
#include "CoreMinimal.h"
#include "../Util/FileIO/Logger.h"

#define CVAR_REPLICATED 1 << 0 //variable replicated on server and client
#define CVAR_MOD		1 << 1 //variable controlled by "moderator" permission level on server
#define CVAR_ADMIN		1 << 2 //variable controlled by "admin" permission, which is higher than moderator
#define CVAR_CHEAT		1 << 3 //cheats must be on for this variable to be on
#define CVAR_PERSISTENT 1 << 4 //variable value is saved to/loaded from config
#define CVAR_GOOFY		1 << 5 //variable isn't a "cheat", but... controlled by goofy setting
#define CVAR_NOTIFY		1 << 6 //notify players in chat when variable changes, log it too
#define CVAR_REPORTED	1 << 7 //variable controlled by player in their cfg, but value is known to server

enum class EConVarType {
	INT = 0,
	NAME = 1,
	BOOL = 2,
	FLOAT = 3
};

class ConVar {
private:
	static TMap<FName, ConVar*> s_stringToCvar;
	static void RegisterCvar(ConVar* cvar);

	int m_iFlags;

	FName m_cvarName;
	FText m_tooltip;
	
	EConVarType m_eType;
	union {
		int m_iVal;
		char* m_pszVal;
		float m_fVal;
		bool m_bVal;
	};

	//For freeing string memory from the heap when necessary
	void CheckDeleteStringValue() {
		if (m_eType == EConVarType::NAME) {
			delete[] m_pszVal;
		}
	}

	bool CheckCanChange();

	//These check if we need to notify everyone about the new value
	void CheckNotifyChange(bool value);
	void CheckNotifyChange(int value);
	void CheckNotifyChange(float value);
	void CheckNotifyChange(const char* psz);
	
public:

	EConVarType GetType() const { return m_eType; }

	inline bool HasFlag(int flag) const { return !!(m_iFlags && flag); }

	int GetValueInt() const {
		return m_iVal;
	}

	const char* GetValuePszString() const {
		return m_pszVal;
	}

	float GetValueFloat() const {
		return m_fVal;
	}

	bool GetValueBool() const {
		return m_bVal;
	}

	void SetValue(bool value) {
		if (!CheckCanChange()) {
			return;
		}
		CheckDeleteStringValue();
		m_eType = EConVarType::BOOL;
		m_bVal = value;
	}

	void SetValue(const char* psz) {
		if (!CheckCanChange()) {
			return;
		}
		CheckDeleteStringValue();
		m_eType = EConVarType::NAME;

		int len = strlen(psz);
		m_pszVal = new char[len + 1]; // +1 for null terminator
		strcpy_s(m_pszVal, len + 1, psz);
		m_pszVal[len] = 0;
	}

	void SetValue(const FName& str) {
		SetValue(NAME_TO_ANSI(str));
	}

	void SetValue(const FString& str) {
		SetValue(CStr(str));
	}

	void SetValue(int value) {
		if (!CheckCanChange()) {
			return;
		}
		CheckDeleteStringValue();
		m_eType = EConVarType::INT;
		m_iVal = value;
	}

	void SetValue(float value) {
		CheckDeleteStringValue();
		m_eType = EConVarType::FLOAT;
		m_fVal = value;
	}

	//Constructors - each adds the constructed ConVar to a global map
	ConVar(const FName& name, int flags, const char* pszVal, const FString& tooltip);
	ConVar(const FName& name, int flags, int val, const FString& tooltip);
	ConVar(const FName& name, int flags, float val, const FString& tooltip);
	ConVar(const FName& name, int flags, bool val, const FString& tooltip);

	//Global accessors
	static ConVar* FindCvarByName(const FName& str);

	static void GetAllCvars(TArray<TPair<FName, ConVar*>>& out);
};

class ConCommand {
protected:
	static TMap<FName, ConCommand*> s_stringToCommand;
	static void RegisterCommand(ConCommand* cmd);

	int m_iFlags;

	FName m_cmdName;
	FText m_tooltip;
	
	virtual void RunCommandActual(int argc, TArray<char*> args) = 0;

public:
	static void ExecuteCommand(const char* cmd); //runs a command as if entered into console
	static void ExecuteCommand(int argc, TArray<char*> args); //runs a command as if entered into console

	static ConCommand* FindCommandByName(const FName& str);

	ConCommand(const FName& name, int flags, const FString& tooltip);
	virtual ~ConCommand() {}
};

#define CON_COMMAND(name, flags, tooltip) \
class ConCommand_##name : ConCommand { \
public: \
	ConCommand_##name() : ConCommand(#name, flags, tooltip) {} \
	void RunCommandActual(int argc, TArray<char*> args) override; \
	~ConCommand_##name() {} \
}; \
void ConCommand_##name::RunCommandActual(int argc, TArray<char*> args)

class Console {
	
};
