#include "CoreMinimal.h"
#include "ConsoleSystem.h"

ConVar sr_cheats("sr_cheats", CVAR_ADMIN | CVAR_NOTIFY | CVAR_REPLICATED, false, "Whether or not to allow built-in cheats on the server.");
ConVar sr_goofy("sr_goofy", CVAR_MOD | CVAR_NOTIFY | CVAR_REPLICATED, false, "Whether or not to allow goofy commands on the server.");

TMap<FString, ConVar*> ConVar::s_stringToCvar;

void ConVar::RegisterCvar(ConVar* cvar) {
	if (s_stringToCvar.Contains(cvar->m_cvarName)) {
		NLogger::Fatal("DUPLICATE cvar %s", CStr(cvar->m_cvarName));
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
		Msg("Cvar %s has changed to %i", CStr(m_cvarName), value);
		Log("Cvar %s has changed to %i", CStr(m_cvarName), value);
	}
}

void ConVar::CheckNotifyChange(float value) {
	if (m_iFlags & CVAR_NOTIFY) {
		Msg("Cvar %s has changed to %f", CStr(m_cvarName), value);
		Log("Cvar %s has changed to %f", CStr(m_cvarName), value);
	}
}

void ConVar::CheckNotifyChange(const char* psz) {
	if (m_iFlags & CVAR_NOTIFY) {
		Msg("Cvar %s has changed to %s", CStr(m_cvarName), psz);
		Log("Cvar %s has changed to %s", CStr(m_cvarName), psz);
	}
}

ConVar::ConVar(const FString& name, int flags, const char* pszVal, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = tooltip;

	m_eType = EConVarType::STRING;
	int len = strlen(pszVal);
	m_pszVal = new char[len + 1]; // +1 for null terminator
	strcpy_s(m_pszVal, len + 1, pszVal);
	m_pszVal[len] = 0;

	RegisterCvar(this);
}

ConVar::ConVar(const FString& name, int flags, int val, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = tooltip;

	m_eType = EConVarType::INT;
	m_iVal = val;

	RegisterCvar(this);
}

ConVar::ConVar(const FString& name, int flags, float val, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = tooltip;

	m_eType = EConVarType::FLOAT;
	m_fVal = val;

	RegisterCvar(this);
}

ConVar::ConVar(const FString& name, int flags, bool val, const FString& tooltip) {
	m_cvarName = name;
	m_iFlags = flags;
	m_tooltip = tooltip;

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