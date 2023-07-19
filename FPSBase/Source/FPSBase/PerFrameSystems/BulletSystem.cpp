#include "BulletSystem.h"
#include "../Util.h"
#include "../PODT/DamageInfo.h"
#include "../FPSBaseCharacter.h"
#include "../Profiles/WeaponDataProfile.h"

#define BULLET_FLAGS CVAR_ADMIN | CVAR_REPLICATED | CVAR_NOTIFY

ConVar sr_simulatedbullets_gravity("sr_simulatedbullets_gravity", BULLET_FLAGS, true, "");

ConVar sr_simulatedbullets_flex("sr_simulatedbullets_flex", BULLET_FLAGS, false,
	"Whether to use sr_simulatedbullets_freq and sr_simulatedbullets_rwc else fall on defaults (faster)"); // BG2 - VisualMelon

ConVar sr_simulatedbullets_freq("sr_simulatedbullets_freq", BULLET_FLAGS, 1000, 
	"Update frequency for simulated bullets."); // BG2 - VisualMelon

ConVar sr_simulatedbullets_rwc("sr_simulatedbullets_rwc", BULLET_FLAGS, 10,
	"Ray Wait Count."); // BG2 - VisualMelon

ConVar sr_simulatedbullets("sr_simulatedbullets", BULLET_FLAGS, false,
	"EXPERIMENTAL!\nWhen non-zero, makes all firearms shoot \"real\" bullets.");

//ConVar sr_arcscanbullets("sr_arcscanbullets", "1", FCVAR_NOTIFY | FCVAR_REPLICATED | FCVAR_DEMO,
	//"Like regular hitscan bullets, only traced along the same arc a sr_simulatedbullets bullet would take.");

ConVar sr_simulatedbullets_drag("sr_simulatedbullets_drag", BULLET_FLAGS, 0.00003f, 
	"Tweak this value to affect how fast the speed and thus damage of bullets drop off with distance.\n\tLower values => more damage over distance");

ConVar sr_simulatedbullets_show_trajectories("sr_simulatedbullets_show_trajectories", BULLET_FLAGS, false,
	"Draw trajectories of the bullets? Useful for adjusting their settings");

ConVar sr_simulatedbullets_show_trajectories_timeout("sr_simulatedbullets_show_trajectories_timeout", BULLET_FLAGS, 10,
	"How long the trajectories stay, in seconds");

//ConVar sr_bullettracers("sr_bullettracers", "1", FCVAR_NOTIFY | FCVAR_REPLICATED, "Do bullets draw tracers behind them?");


SimpleBullet::SimpleBullet(
	FVector position, 
	FQuat angle, 
	int iDamage, 
	FPSBaseCharacter* owner, 
	WeaponDef* weapon,
	int damageTypeOverride) :
	m_position(position),
	m_angle(angle),
	m_iDamage(iDamage),
	m_owner(owner),
	m_weapon(weapon),
	m_iDamageTypeOverride(damageTypeOverride) {}

SimulatedBullet::SimulatedBullet(
	FVector	position,
	FQuat angle,
	int iDamage,
	float flConstantDamageRange,
	float flRelativeDrag,
	float flMuzzleVelocity,
	float flDamageDropoffMultiplier,
	FPSBaseCharacter* owner,
	WeaponDef* weapon,
	bool bPenetrateFlesh,
	int damageTypeOverride) :
	m_vPosition(position),
	m_angle(angle),
	m_iDamage(iDamage),
	m_flConstantDamageRange(flConstantDamageRange),
	m_flRelativeDrag(flRelativeDrag),
	m_flMuzzleVelocity(flMuzzleVelocity),
	m_flDamageDropoffMultiplier(flDamageDropoffMultiplier),
	m_owner(owner),
	m_weapon(weapon),
	m_bPenetrateFlesh(bPenetrateFlesh),
	m_iDamageTypeOverride(damageTypeOverride) {

	FVector vecDir = angle.Vector();

	m_vTrajStart = m_vLastPosition = m_vPosition = position;
	m_vVelocity = vecDir * flMuzzleVelocity;
	m_flDyingTime = g_globals.time + /*LIFETIME*/ 6.0f;
	m_flDamageDropoffMultiplier = flDamageDropoffMultiplier;
	m_bHasPlayedNearMiss = false;
	m_flHitDropProbability = 0.f; //((CBaseBG2Weapon*)m_pWeapon)->GetHitDropProbability();
	m_iBounces = 0;
	
	resetRwc(sr_simulatedbullets_flex.GetValueBool());
}

void SimulatedBullet::resetRwc(bool flex)
{
	if (flex)
		rwc = sr_simulatedbullets_rwc.GetValueInt();
	else
		rwc = BULLET_SIMULATION_RWC;
}

// BG2 - VisualMelon - Swapped in this version of Think which uses the rwcounter
bool SimulatedBullet::Think(float dt, bool flex)
{
	//dead?
	if (g_globals.time > m_flDyingTime)
		return false;

	UpdateVelocity(dt);
	DoNearMiss(); // BG2 - VisualMelon - This code is ANGRY (And all #if 0'd out)

	//advance position and trace
	rwc--;
	if (rwc <= 0) {
		FVector step = m_vVelocity * dt;
		m_vPosition += step;
		//Msg("(%.2f,%.2f,%.2f)\n", m_vPosition.x, m_vPosition.y, m_vPosition.z);

		resetRwc(flex);

		bool res = !DoTrace(step);

		m_vLastPosition = m_vPosition;

		return res;
	}
	else {
		m_vPosition += m_vVelocity * dt;
		return true;
	}
}

void SimulatedBullet::UpdateVelocity(float dt)
{
	//apply drag
	FVector	vecDir = m_vVelocity;
	extern	ConVar	sr_simulatedbullets_drag,
		sr_gravity;

	float speed2 = vecDir.SquaredLength();
	float speed = vecDir.Length(),
		drag = sr_simulatedbullets_drag.GetValueFloat() * m_flRelativeDrag;

	speed -= drag * speed2 * dt; //speed exponentially increases drag

	m_vVelocity = vecDir * speed;
	m_vVelocity.Z -= sr_gravity.GetValueFloat() * sr_simulatedbullets_gravity.GetValueFloat() * dt;
}

static SLineDrawParams g_trajectoryDrawParams = SLineDrawParams(FCOLOR_WHITE, 3.f, 10.f);

//return true if we hit something, false otherwise
bool SimulatedBullet::DoTrace(const FVector& step)
{
	FHitResult tr;
	AActor* ignored[] = { (AActor*)m_owner };
	UTIL_TraceLine(tr, m_vLastPosition, m_vPosition, ignored);

	if (sr_simulatedbullets_show_trajectories.GetValueBool()) {
		if (tr.bBlockingHit) g_trajectoryDrawParams.m_Color = FCOLOR_RED;
		else g_trajectoryDrawParams.m_Color = FCOLOR_WHITE;

		UTIL_DrawLine(m_vLastPosition, m_vPosition, &g_trajectoryDrawParams);
	}
	
		//UTIL_TraceLine(tr, m_vLastPosition, m_vPosition, ignored);
		//NDebugOverlay::Line(m_vLastPosition, m_vPosition, tr.DidHit() ? 0 : 255, tr.DidHitWorld() ? 0 : 255, tr.DidHitWorld() ? 255 : 0, true, sr_simulatedbullets_show_trajectories_timeout.GetValueFloat());

	if (!tr.bBlockingHit)
		return false;

	FVector vecDir = m_vVelocity;
	float speed = vecDir.Length();

	//UTIL_ImpactTrace(&tr, DMG_BULLET);

	//if we hit something that's not an actor, we hit the world
	if (tr.bBlockingHit && !tr.GetActor()) {
		//miss (hit world)
		//don't bounce more than twice
		if (m_iBounces < 2) {
			//BG2 - VisualMelon - nastyness to prevent exploitation for the minute
			m_vPosition = tr.Location;
			//

			//BG2 - Tjoppen - Bullet.HitWorld
			//EmitSound( "Bullet.HitWorld" );

			// See if we should reflect off this surface
			
			float hitDot = FVector::DotProduct(tr.ImpactNormal, -vecDir.GetUnsafeNormal());

			// BG2 - BP original was( hitDot < 0.5f ) but a musket ball should not bounce off walls if the angle is too big
			//BG2 - Tjoppen - don't ricochet unless we're hitting a surface at a 60 degree horisontal angle or more
			//					this is a hack so that bullets don't ricochet off the ground
			//if ( ( hitDot < 0.2f ) && ( speed > 100 ) )
			if (hitDot < 0.2f && tr.ImpactNormal.Z < 0.5f) {
				m_iBounces++;

				//reflect, slow down 25%
				m_vVelocity = .75f * (m_vVelocity - 2.f * m_vVelocity.Dot(tr.ImpactNormal) * tr.ImpactNormal);

				//since we reflected, pretend we didn't hit anything
				return false;
			}
		}
	}
	else if (tr.bBlockingHit) {
		DoHit(tr, speed);
	}

	return true;
}

bool SimulatedBullet::DoHit(FHitResult& tr, float speed) {
	//check if actor we hit is damageable
	FIDamageable* pVictim = dynamic_cast<FIDamageable*>(tr.GetActor());
	if (!pVictim) {
		return true;
	}

	//build basic damage object and check if we can damage
	FDamageInfo di = FDamageInfo(0, pVictim, DMG_BULLET, (AActor*)m_owner, NULL, m_weapon);

	if (!pVictim->ShouldTakeDamage(di)) {
		return true;
	}


	//pass random hit drop
	//if ((tr.endpos - m_vTrajStart).Length() > 2200.f && RndBool(m_flHitDropProbability))
		//return false; //pretend we didn't hit anything and pass through, this will reward situations where an LB line outflannks another

	//we hit something that can be damaged
	//trace through arms
	TraceThroughArms(&tr);

	//calculate damage
	int dmg;
	if ((tr.Location - m_vTrajStart).Length() < m_flConstantDamageRange)
		dmg = m_iDamage;
	else {
		dmg = (int)(m_iDamage * speed * speed / (m_flMuzzleVelocity * m_flMuzzleVelocity));
		dmg = m_iDamage - (m_iDamage - dmg) * m_flDamageDropoffMultiplier; //scale damage dropof\f
	}

	di.SetDamage(dmg);

	//TODO positional blood effects
	pVictim->TakeDamage(di, true); //skip checks, we checked already



	//Adrian: keep going through the glass.
	//TODO add glass physics material
	/*if (tr.m_pEnt->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS)
		return false;*/

		//keep going if our bullet penetrates flesh
	if (m_bPenetrateFlesh) {
		//ignore constant damage range for future hits
		m_flConstantDamageRange = 0.f;

		//cut velocity by half or so
		m_vVelocity = m_vVelocity * 0.9f;

		return false; //pretend that we hit nothing
	}
}

/**
	* Trace a few more units ahead to see if something other than an arm is behind what we hit.
	* This fixes the "kevlar arms" issue.
	*/
void SimulatedBullet::TraceThroughArms(FHitResult* tr) {
	//we only care about hitting arms
	if (FCString::Atoi(WCStr(tr->BoneName.ToString())) < HITGROUP_LEFTARM)
		return;

	FVector dir = m_vPosition - m_vLastPosition;
	dir = dir.GetUnsafeNormal();

	FHitResult tr2;
	tr2.Init();
	FVector start = tr->Location;
	FVector end = tr->Location + dir * 64; //64 centimeters

	//if (sr_simulatedbullets_show_trajectories.GetValueBool())
		//NDebugOverlay::Box(start, FVector(-0.1f, -0.1f, -0.1f), FVector(0.1f, 0.1f, 0.1f), 0, 255, 255, 255, sr_simulatedbullets_show_trajectories_timeout.GetValueFloat());

	UTIL_TraceLine(tr2, start, end);

	//TODO sort out hitgroup comparisons!
	if (tr2.GetActor() == tr->GetActor() 
		&& FCString::Atoi(WCStr(tr2.BoneName.ToString())) < HITGROUP_LEFTARM) {
		//hit non-generic non-arm hitgroup on the same player
		//replace trace with the new one - we probably hit the chest or stomach instead
		*tr = tr2;
		return;
	}
}

static TArray<SimulatedBullet> activeBullets;
static const float step = 1.f / BULLET_SIMULATION_FREQUENCY;

void BulletSystem::Init() {

}

bool BulletSystem::SpawnSimpleBullet(const SimpleBullet& bullet) {
	//simple bullets aren't simulated, trace and do damage right away
	FHitResult tr;
	AActor* ignored[] = { (AActor*)bullet.m_owner };
	float maxRange = bullet.m_weapon->m_Attackinfos[0].m_flRange;
	FVector end = bullet.m_position + bullet.m_angle.Vector() * bullet.m_weapon->m_Attackinfos[0].m_flRange;
	UTIL_TraceLine(tr, bullet.m_position, end, ignored);

	if (sr_simulatedbullets_show_trajectories.GetValueBool()) {
		if (tr.bBlockingHit && tr.GetActor()) g_trajectoryDrawParams.m_Color = FCOLOR_RED;
		else g_trajectoryDrawParams.m_Color = FCOLOR_WHITE;

		UTIL_DrawLine(bullet.m_position, end, &g_trajectoryDrawParams);
	}

	//TODO impact effects!
	if (!tr.bBlockingHit)
		return false;

	//check if actor we hit is damageable
	FIDamageable* pVictim = dynamic_cast<FIDamageable*>(tr.GetActor());
	if (!pVictim) {
		return true;
	}

	//build basic damage object and check if we can damage
	FDamageInfo di = FDamageInfo(bullet.m_iDamage, pVictim, DMG_BULLET, (AActor*)bullet.m_owner, NULL, bullet.m_weapon);

	di.ApplyToVictim();

	//TODO positional blood effects
	
	//TODO going through glass
}

void BulletSystem::SpawnSimulatedBullet(SimulatedBullet&& bullet)
{
	//TODO add lag compensation
	/*
	bool flex = sr_simulatedbullets_flex.GetValueBool(); // BG2 - VisualMelon - store this, I'm not sure what lookup times are like

	
	float targetTime = 
	float delta = g_globals->curtime - targetTime;

	float curStep = step;
	if (flex)
		curStep = 1.0 / sr_simulatedbullets_freq.GetValueFloat();

	if (delta > 0) {
		for (float t = targetTime; t < g_globals.time; t += curStep) {
			float dt = min(curStep, g_globals.time - t);

			// BG2 - VisualMelon - only do lag compensation when we are tracing
			bool doLagCompensation = (bullet.rwc <= 1); // BG2 - VisualMelon - rwc <= 1 because it gets --'d
			if (doLagCompensation)
				

			bool ret = bullet.Think(dt, flex);

			if (doLagCompensation)
				

			//dead. no need to keep simulating
			if (!ret)
				return;
		}
	}*/

	//bullet is still alive - add it to the list for futher simulation
	activeBullets.Emplace(bullet);
}

// BG2 - VisualMelon - seems to consume about .3% as much time as the lastFrameTime (assuming same units)
//                     varies wildley, can be as high as 40%
//                     tests done with packed local server of bots, mesuring british light inf/shot
void BulletSystem::Tick(float deltaSeconds) {
	bool flex = sr_simulatedbullets_flex.GetValueBool(); // BG2 - VisualMelon - store this, I'm not sure what lookup times are like

	// BG2 - VisualMelon - Timing
	//double startTime = Plat_FloatTime();

	//double step = 1.0 / sr_simulatedbullets_freq.GetValueFloat(); // don't use for production

	float curStep = step;
	if (flex)
		curStep = 1.0 / sr_simulatedbullets_freq.GetValueFloat();

	float lastFrameTime = deltaSeconds;
	for (float t = 0; t < lastFrameTime; t += curStep) {
		float dt = fminf(curStep, lastFrameTime - t);

		int b = activeBullets.Num();
		if (b == 0)
			break; // BG2 - VisualMelon - stop looping (best case insignificant optimisation useful for testing)

		//have all bullets think. remove those that hit something or have timed out
		for (b = b - 1; b >= 0; b--) {
			if (!activeBullets[b].Think(dt, flex)) { // bullet dead
				// BG2 - VisualMelon - MicroOptimisation, cheaper to swap out than to erase in the middle (in my tests atleast)
				//                   - Also makes the for loop look tidier (and it's backward now, faster if no sneaky optimisations)
				activeBullets[b] = activeBullets[(int)activeBullets.Num() - 1]; // move back in-place
				activeBullets.Pop();
				//activeBullets.erase(activeBullets.begin() + b);
			}
		}
	}

	//double endTime = Plat_FloatTime();
	//double btime = endTime - startTime;
	//if (flex && btime > 0.000001)
	//	DevMsg("Updating Bullets for %f of %f (%f)\n", btime, lastFrameTime, btime / lastFrameTime);
}