#pragma once

#include "GameFramework/HUD.h"
#include "ChatMessage.generated.h"

USTRUCT()
struct FChatMsg {
	GENERATED_BODY()

	UPROPERTY()
	int32 m_type;

	UPROPERTY()
	FText m_username;

	UPROPERTY()
	FText m_text;

	FText m_tTimestamp;

	double m_dTimestamp;

	void Init(int32 type, FText username, FText text) {
		m_type = type;
		m_username = username;
		m_text = text;
	}

	void SetTime(FText tTimestamp, double dTimestamp) {
		m_tTimestamp = tTimestamp;
		m_dTimestamp = dTimestamp;
	}

	void Destroy() {
		m_type = m_dTimestamp = 0;
	}
};