#pragma once
#include "CoreMinimal.h"

#define CVAR_REPLICATED 1 << 0 //variable replicated on server and client
#define CVAR_MOD		1 << 1 //variable controlled by "moderator" permission level on server
#define CVAR_ADMIN		1 << 2 //variable controlled by "admin" permission, which is higher than moderator
#define CVAR_CHEAT		1 << 3 //cheats must be on for this variable to be on
#define CVAR_PERSISTENT 1 << 4 //variable value is saved to/loaded from config
#define CVAR_GOOF		1 << 5 //variable isn't a "cheat", but... controlled by goofy setting
#define CVAR_NOTIFY		1 << 6 //notify players in chat when variable changes
#define CVAR_REPORTED	1 << 7 //variable controlled by player in their cfg, but value is known to server


class ConVar {

};

class ConCommand {

};
