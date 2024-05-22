#include "GameModeCvars.h"
#include "Systems/ConsoleSystem.h"

#define CVAR_FLAGS (CVAR_MOD | CVAR_REPLICATED | CVAR_NOTIFY)

ConVar gr_fullcap_bonus("gr_fullcap_bonus", CVAR_FLAGS, 200, "Number of points to deliver on fullcap");

ConVar gr_friendlyfire("gr_friendlyfire", CVAR_FLAGS, true, "Whether or not to allow friendly fire");
ConVar gr_friendlyfire_grenades("gr_friendlyfire_grenades", CVAR_FLAGS, true, "Whether or not to allow friendly fire, grenade specific");
ConVar gr_friendlyfire_cannon("gr_friendlyfire_cannon", CVAR_FLAGS, true, "Whether or not to allow friendly fire, cannon specific");

ConVar gr_gamemode("gr_gamemode", CVAR_FLAGS, 0, "0 = Skirmish, 1 = Tickets, 2 = LMS, 3 = Linebattle");
ConVar gr_respawntime_a("gr_respawntime_a", CVAR_FLAGS, 10, "Interval between respawn waves for attackers");
ConVar gr_respawntime_d("gr_respawntime_d", CVAR_FLAGS, 10, "Interval between respawn waves for defenders");
ConVar gr_competitive("gr_competitive", CVAR_FLAGS, 10, "0 = casual, 1 = competitive, 2 = super competitive. Alters several gamerules");
ConVar gr_timelimit("gr_timelimit", CVAR_FLAGS, 15, "Game length in minutes, when not using rounds");

ConVar gr_tickets_a("gr_tickets_a", CVAR_FLAGS, 10, "Number of tickets for the attacking team.");
ConVar gr_tickets_d("gr_tickets_d", CVAR_FLAGS, 10, "Number of tickets for the defending team.");
ConVar gr_tickets_a_drain("gr_tickets_a_drain", CVAR_FLAGS, 10, "Ticket drain amount for attacking team per interval");
ConVar gr_tickets_d_drain("gr_tickets_d_drain", CVAR_FLAGS, 10, "Ticket drain amount for defending team per interval");
ConVar gr_tickets_interval("gr_tickets_interval", CVAR_FLAGS, 10, "Time between ticket drains");

ConVar gr_rounds("gr_rounds", CVAR_FLAGS, 0, "If non-zero, game has this many rounds");
ConVar gr_rounds_timelimit("gr_rounds_timelimit", CVAR_FLAGS, 300, "Round length in seconds, when mp_rounds > 0");
ConVar gr_rounds_autoswitch("gr_rounds_autoswitch", CVAR_FLAGS, 0, "Whether or not to autoswitch teams. 0 = no, 1 = at the half, 2 = every other round");

ConVar gr_classlimits("gr_classlimits", CVAR_FLAGS, -1, "Whether or not to enforce class limits. -1 = no limits, 0 = infantry only, 1+ = limit for non-infantry classes");
ConVar gr_classlimits_scale_interval("gr_classlimits_scale_interval", CVAR_FLAGS, 8, "Each time this many players join, add 1 to the class limits. 0 to disable.");

ConVar sr_nextmap("sr_nextmap", CVAR_FLAGS, "", "Which map should be played next");
