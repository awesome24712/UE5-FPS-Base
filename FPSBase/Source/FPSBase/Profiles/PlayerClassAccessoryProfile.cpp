#include "PlayerClassAccessoryProfile.h"

PlayerClassAccessoryProfile::PlayerClassAccessoryProfile() {
	m_eType = EAccessoryType::CLASS_PERK;
	m_eFilter = 0;
	m_flWeight = m_flSpeedModifier = m_flMaxWeightModifier = 0;

	JT_START_BINDING("playerClassAccessory", PlayerClassAccessoryProfile);
	JT_BIND_INT(m_eType, "type", true);
	JT_BIND_INT(m_eFilter, "filter", true);
	JT_BIND_FLOAT(m_flWeight, "weight", false);
	JT_BIND_FLOAT(m_flSpeedModifier, "speedMod", false);
	JT_BIND_FLOAT(m_flMaxWeightModifier, "maxWeightMod", false);
	JT_FINISH_BINDING();
}