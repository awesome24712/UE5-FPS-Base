#pragma once
//TODO find an entry point to seed this!
void RndSeed(int seed);
void RndSeed(uint64 seed);

int RndInt();

int RndInt(int min, int max);

float RndFloat();

float RndFloat(float min, float max);

inline bool RndBool(float chance = 0.5f) {
	return RndFloat() < chance;
}

//This is used when both client and server need to generate the same random number
//so they just pass in a whole bunch of randomish runtime info to get a pseudo-random number they can agree on
//template<int N>
#define NUM_RND_CLIENT_SERVER_SYNC_SEEDS 4
void RndSeed(int seeds[NUM_RND_CLIENT_SERVER_SYNC_SEEDS]);