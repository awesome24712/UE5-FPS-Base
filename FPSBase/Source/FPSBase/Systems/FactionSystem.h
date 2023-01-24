#pragma once
#include "CoreMinimal.h"

//Faction system - note that faction != team, i.e. different factions can be assigned to different teams
class Faction {
private:
	int m_iFactionNumber; //shorthand identifier for faction

	FString m_sFactionCodeName; //name as it appears in console and logs, unlocalized
	FString m_sFactionDisplayName; //name as to be shown to users in generic cases, localized
	FColor m_cColor; //color representation for this team

public:
	Faction(int iNumber, FString codeName, FString displayName, FColor factionColor);

	int GetFactionNumber() const { return m_iFactionNumber; }
	const FString& GetCodeName() const { return m_sFactionCodeName; }
	const FString& GetCodeName() const { return m_sFactionDisplayName; }
	const FColor& GetColor() const { return m_cColor; }

	static TArray<const Faction*> GetFactions() const;
};
