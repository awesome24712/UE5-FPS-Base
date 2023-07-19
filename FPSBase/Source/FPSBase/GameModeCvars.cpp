#include "GameModeCvars.h"
#include "Systems/ConsoleSystem.h"



ConVar gr_friendlyfire("gr_friendlyfire", CVAR_MOD | CVAR_REPLICATED | CVAR_NOTIFY, true, "Whether or not to allow friendly fire");
ConVar gr_friendlyfire_grenades("gr_friendlyfire_grenades", CVAR_MOD | CVAR_REPLICATED | CVAR_NOTIFY, true, "Whether or not to allow friendly fire, grenade specific");
ConVar gr_friendlyfire_cannon("gr_friendlyfire_cannon", CVAR_MOD | CVAR_REPLICATED | CVAR_NOTIFY, true, "Whether or not to allow friendly fire, cannon specific");
