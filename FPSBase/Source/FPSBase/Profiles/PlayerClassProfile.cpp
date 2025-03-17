#include "PlayerClassProfile.h"

ConVar mp_year("mp_year", CVAR_MOD | CVAR_NOTIFY, 1779, "Year setting controls which weapons are available, for historical accuracy. Used only if mp_year_accuracy = 1");
ConVar mp_year_accuracy("mp_year_accuracy", CVAR_MOD | CVAR_NOTIFY, false, "Controls whether or not to use year-by-year historical weapon accuracy.");

//bool GunKit::AvailableForCurrentYear() const {
//	//return true;
//	if (mp_year_accuracy.GetValueBool() && mp_year.GetValueBool()) {
//		int year = mp_year.GetValueInt();
//		return (year >= m_iMinYear && year <= m_iMaxYear);
//	}
//	else {
//		return !m_bDisabledOnHistoricityDisabled || mp_year.GetValueInt() == 0;
//	}
//}

PlayerClassProfile::PlayerClassProfile() {

	JT_START_BINDING("playerClass", PlayerClassProfile);
	JT_BIND_OBJECT(m_profile, "profile", true);
	JT_BIND_FLOAT(m_flWeightCapacity, "weightCapacity", true);
	JT_BIND_STRING(m_sAbbreviation, "abbreviation", true);
	JT_BIND_STRING(m_sDisplayName, "displayName", true);
	JT_FINISH_BINDING();

}


