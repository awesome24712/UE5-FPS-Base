#include "TypeConversions.h"
#include "../Util.h"

FColor StringToColor(const FString& s) {
	char arr[16];
	strcpy_s(arr, CStr(s));
	uint8 i = 0, c = 1;
	union { struct { uint8 R, G, B, A; }; uint32 result; uint8 bits[4]; };
	result = 0;

	while (i < 16 && arr[i] && c < 4) {
		if (arr[i] == ',') {
			bits[c] = i + 1; //the initial value of c is 1 because we already know R begins at i == 0
			c++;
			arr[i] = 0;
		}
		i++;
	}

	//
	R = atoi(&arr[R]);
	G = atoi(&arr[G]);
	B = atoi(&arr[B]);
	A = atoi(&arr[A]);

	//FColor uses BGRA so swap R and B
	i = R;
	R = B;
	B = i;

	return FColor(result);
}