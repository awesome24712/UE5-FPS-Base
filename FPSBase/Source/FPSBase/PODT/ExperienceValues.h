#pragma once
#include "CoreMinimal.h"
#include "../Util.h"


struct FExperienceValues : public IJsonBindable {

	uint8 m_iAttack; //melee damage output modifier, 1 unit = 1 damage

	uint8 m_iDefense; //damage recieved modifier, 1 unit = 1 damage

	uint8 m_iReach; //melee range modifier, 1 unit = ?

	uint8 m_iStamina; //max stamina quantity modifier, 1 unit = 1 stamina

	uint8 m_iAccuracy; //accuracy modifier, 1 unit = ?

	uint8 m_iSpeed; //speed modifier, 1 unit = 1%

	FExperienceValues() {

	}

};