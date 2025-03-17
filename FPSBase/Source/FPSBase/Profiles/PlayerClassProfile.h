#pragma once
#include "../Util.h"
#include "WeaponDefProfile.h"
#include "KitAccessoryProfile.h"

class PlayerClassProfile : public IJsonBindable {

	//a lot of class-specific stats can just be re-used from KitAccessoryProfile
private:
	KitAccessoryProfile	m_profile;
	float				m_flWeightCapacity;
	FString				m_sAbbreviation;
	FString				m_sDisplayName;

public:
	PlayerClassProfile();

	//--------------------------------------------
	// Getters
	//--------------------------------------------
	const KitAccessoryProfile* GetProfile() const { return &m_profile; }

	float GetWeightCapacity() const { return m_flWeightCapacity; }

	const FString& GetAbbreviation() const { return m_sAbbreviation; }

	/*float GetDefaultMaxSpeed() const { 
		return BASE_SPEED_ALL_CLASSES + (SPEED_INCREMENT_PER_AV * m_profile.m_adjustments.m_iSpeed);
	}*/
};