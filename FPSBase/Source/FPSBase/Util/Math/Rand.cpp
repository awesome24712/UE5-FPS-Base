#include "Rand.h"
#include "../Predefs.h"

static int a = 9158152, b = 14257153;


#define abs(a) ((a) < 0 ? -(a) : (a))

void RndSeed(int seed) {
	a = seed;
}

void RndSeed(ulong seed) {
	a = (int)seed;
	b = (int)(seed >> 32ULL);
}

void RndSeed(int seeds[NUM_RND_CLIENT_SERVER_SYNC_SEEDS]) {
	int sum = 0;
	for (int i = 0; i < NUM_RND_CLIENT_SERVER_SYNC_SEEDS; i++) {
		sum += ~seeds[i];
	}
	a = sum;
	b = sum ^ 14257153;

	//run it a few time to let chaos theory happen
	RndInt(); RndInt(); RndInt(); RndInt();
}


int RndInt()
{
	a += b + (a >> 5);
	b += a + (b >> 11);

	return abs(b);
}

int RndInt(int min, int max) {
	return min + (RndInt() % (max - min + 1));
}

//Returns a float between 0 and 1
float RndFloat() {
	float result = (1.0f * RndInt()) / INT_MAX;
	return abs(result);
}

float RndFloat(float min, float max) {
	float result = (1.0f * RndInt()) / INT_MAX;
	if (result < 0)
		result = -result;
	result = min + result * (max - min);
	return result;
}

