#include "ConsoleSystem.h"

ConVar sr_cheats("sr_cheats", CVAR_ADMIN | CVAR_NOTIFY | CVAR_REPLICATED, false, "Whether or not to allow built-in cheats on the server.");
ConVar sr_goofy("sr_goofy", CVAR_MOD | CVAR_NOTIFY | CVAR_REPLICATED, false, "Whether or not to allow goofy commands on the server.");

TMap<FString, ConVar*> ConVar::s_stringToCvar;

void ConVar::RegisterCvar(ConVar* cvar) {
	if (s_stringToCvar.Contains(cvar->m_cvarName)) {
		NLogger::Fatal("DUPLICATE cvar %s", WCStr(cvar->m_cvarName));
	}
	else {
		s_stringToCvar.Add(cvar->m_cvarName, cvar);
	}
}


bool ConVar::CheckCanChange() {
	bool cheats = FindCvarByName("sr_cheats")->GetValueBool();
	if (!cheats && HasFlag(CVAR_CHEAT)) {
		return false;
	}

	bool goofy = FindCvarByName("sr_goofy")->GetValueBool();
	if (!goofy && HasFlag(CVAR_GOOFY)) {
		return false;
	}
	return true;
}

void ConVar::CheckNotifyChange(bool value) {
	CheckNotifyChange((int)value);
}

void ConVar::CheckNotifyChange(int value) {
	if (m_iFlags & CVAR_NOTIFY) {
		Msg("Cvar %s has changed to %i", WCStr(m_cvarName), value);
		Log("Cvar %s has changed to %i", WCStr(m_cvarName), value);
	}
}

void ConVar::CheckNotifyChange(float value) {
	if (m_iFlags & CVAR_NOTIFY) {
		Msg("Cvar %s has changed to %f", WCStr(m_cvarName), value);
		Log("Cvar %s has changed to %f", WCStr(m_cvarName), value);
	}
}

void ConVar::CheckNotifyChange(const char* psz) {
	if (m_iFlags & CVAR_NOTIFY) {
		Msg("Cvar %s has changed to %s", WCStr(m_cvarName), psz);
		Log("Cvar %s has changed to %s", WCStr(m_cvarName), psz);
	}
}

ConVar::ConVar(const FString& name, int flags, const char* pszVal, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = FText::FromString(tooltip);

	m_eType = EConVarType::NAME;
	int len = strlen(pszVal);
	m_pszVal = new char[len + 1]; // +1 for null terminator
	strcpy_s(m_pszVal, len + 1, pszVal);
	m_pszVal[len] = 0;

	RegisterCvar(this);
}

ConVar::ConVar(const FString& name, int flags, int val, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = FText::FromString(tooltip);

	m_eType = EConVarType::INT;
	m_iVal = val;

	RegisterCvar(this);
}

ConVar::ConVar(const FString& name, int flags, float val, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = FText::FromString(tooltip);

	m_eType = EConVarType::FLOAT;
	m_fVal = val;

	RegisterCvar(this);
}

ConVar::ConVar(const FString& name, int flags, bool val, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = FText::FromString(tooltip);

	m_eType = EConVarType::BOOL;
	m_bVal = val;

	RegisterCvar(this);
}

ConVar* ConVar::FindCvarByName(const FString& str) {
	if (s_stringToCvar.Contains(str)) {
		return s_stringToCvar[str];
	}
	else {
		return NULL;
	}
}

void ConVar::GetAllCvars(TArray<TPair<FString, ConVar*>>& out) {
	out = s_stringToCvar.Array();
}

void ConCommand::ExecuteCommand(const char* cmd) {
	int length = strlen(cmd);
	if (length < 2) {
		return;
	}

	//create a buffer for us to manipulate
	const int BUF_SIZE = 256;
	char buffer[BUF_SIZE];
	strcpy_s(buffer, cmd);

	//figure out what our arguments are
	int argc = 0;
	TArray<char*> args;
	for (int i = 1; i < BUF_SIZE; i++ ) {
		if (buffer[i] != ' ' && buffer[i-1] == ' ') {
			argc++;
			args.Add(&buffer[i]);

			//find the next space character and mark it as null
			for (; i < BUF_SIZE; i++) {
				if (buffer[i] == ' ' || !buffer[i]) {
					buffer[i] = 0;
					break;
				}
			}
		}
	}

	//ensure the end of the buffer is always null
	buffer[BUF_SIZE - 1] = 0;

	//the name of our command is the first arg
	FString scmd = args[0];
	if (s_stringToCommand.Contains(scmd)) {
		ConCommand* cmdObject = s_stringToCommand[scmd];
	} else {
		Msg("Unknown command %s", args[0]);
	}
}

ConCommand* ConCommand::FindCommandByName(const FString& str) {
	if (s_stringToCommand.Contains(str)) {
		return s_stringToCommand[str];
	}
	else {
		return NULL;
	}
}

TMap<FString, ConCommand*> ConCommand::s_stringToCommand;

void ConCommand::RegisterCommand(ConCommand* cvar) {
	if (s_stringToCommand.Contains(cvar->m_cmdName)) {
		NLogger::Fatal("DUPLICATE cvar %s", WCStr(cvar->m_cmdName));
	}
	else {
		s_stringToCommand.Add(cvar->m_cmdName, cvar);
	}
}

ConCommand::ConCommand(const FString& name, int flags, const FString& tooltip) {
	m_cmdName = name;
	m_iFlags = flags;
	m_tooltip = FText::FromString(tooltip);

	RegisterCommand(this);
}

CON_COMMAND(helloworld, 0, "Prints hello world") {
	Msg("Hello world!");
}