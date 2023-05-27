#pragma once
#include "CoreMinimal.h"
#include "../Util/JSON/PropertyBinder.h"

//This class is simply a layer on top of FColor to make it bindable
class BColor : public FColor, public IJsonBindable {
public:
	BColor() {
		JT_START_BINDING("color", BColor);
		JT_BIND_BYTE(R, "r", true);
		JT_BIND_BYTE(G, "g", true);
		JT_BIND_BYTE(B, "b", true);
		JT_BIND_BYTE(A, "a", true);
		JT_FINISH_BINDING();
	}
};