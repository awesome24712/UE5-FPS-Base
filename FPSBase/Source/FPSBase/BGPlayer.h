// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Util/JSON/PropertyBinder.h"
#include "PODT/DamageInfo.h"
#include "Systems/PermissionSystem.h"
#include "Systems/FactionSystem.h"
#include "Profiles/KitAccessoryProfile.h"
#include "Profiles/PlayerClassProfile.h"
//#include "Profiles/WeaponSystemProfile.h"
#include "BGPlayer.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

class KitAccessoryProfile;
class WeaponDef;

class ABGPlayer;
ABGPlayer* GetLocalPlayer();

inline ABGPlayer* ToPlayer(AActor* pActor) { return dynamic_cast<ABGPlayer*>(pActor); }

UCLASS(config=Game)
class ABGPlayer : public ACharacter, public IJsonBindable, public FIDamageable
{
	GENERATED_BODY()

	friend class Faction;
	friend struct FKitLoadout;

	E_BG3_PermissionLevel m_ePermissionLevel = E_BG3_PermissionLevel::PUBBER;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	double m_jumpSpeed = 100.0;
	double m_runSpeed = 100;
	//WeaponSystemProfile* m_pActiveWeaponSystemProfile;

	//Player-configurable options - class, weapon, etc.
	EFactionNumber m_iFaction;
	ETeamNumber m_iTeam;
	UPROPERTY() FKitLoadout m_loadout;
	bool m_bLoadoutChangePending = false;
	const PlayerClass* m_pClass;
	WeaponDef* m_pPrimaryWeaponSelection;
	WeaponDef* m_pSecondaryWeaponSelection;
	WeaponDef* m_pTertiaryWeaponSelection;

	KitAccessoryProfile* m_pPrimaryWeaponPerk;
	KitAccessoryProfile* m_pSecondaryWeaponPerk;

	KitAccessoryProfile* m_pClassSpecificPerk;
	KitAccessoryProfile* m_pPrimaryClassPerk;
	KitAccessoryProfile* m_pSecondaryClassPerk;

	KitAccessoryModifiers m_accumulatedPerks;

	uint8 m_iAmmoIncrement; //5 shots per increment, adds weight too

	//per-life stats
	//uint8 m_iHealth; //health is defined by FIDamageable
	uint8 m_iStamina;
	uint32 m_iRallyFlags;

	//bleeding mechanic variables
	uint8 m_iBandages;
	uint8 m_iHealthToLose; //losing 1 per second
	uint8 m_iHealthLostToBleeding; //to be healed with bandages


public:
	ABGPlayer();

protected:
	virtual void BeginPlay();
	
	/** Primary attack, stab/shoot */
	void OnPrimaryAction();

	void OnSecondaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	void CustomJump();

public:
	FVector GetAimDirection() const;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	void SetupFPV_MeshComponent();


public:


	//------------------------------------------------------------------
	// player management functions
	//------------------------------------------------------------------
	void SwapToPlayerClass(const PlayerClass* pClass, bool bForceNow);


	//------------------------------------------------------------------
	// per-life functions
	//------------------------------------------------------------------
	void Spawn(bool bForce);

	void AccumulatePerks(); //updates m_accumulatedPerks

	void GiveSpawnWeapons();

	void GiveAmmoFull();

	void SetCosmetics(); //ensure we have the right player model

	//------------------------------------------------------------------
	// FIDamageable overrides
	//------------------------------------------------------------------
	virtual void FilterDamage(FDamageInfo& di) override; //called first, gives chance to modify damage before receiving it
	virtual void OnTakeDamage(const FDamageInfo& di) override;
	virtual void OnDeath(const FDamageInfo& di) override;
	virtual void OnTakeDamageDeathBlocked(const FDamageInfo& di) override; //called when we receive non-fatal damage that would have otherwise killed us
	virtual void OnHealed(const FDamageInfo& di) override; //called when damage is negative
	virtual void OnStunned(const FDamageInfo& di) override; //called when hit with stun damage

	//------------------------------------------------------------------
	// movement functions
	//------------------------------------------------------------------
	float GetMaxSpeed(); //big calculation

	void DrainStamina(int drain);


	//------------------------------------------------------------------
	// helper getters
	//------------------------------------------------------------------
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	inline APlayerController& PC() { return *((APlayerController*)Controller.Get()); }


	//WeaponSystemProfile* GetActiveWeaponSystemProfile() const { return m_pActiveWeaponSystemProfile; }

	bool IsBot() const { return false; }
	Faction* GetFaction() const;
	FString GetPlayerName() const { return ""; }
	float GetWeight() const { return m_accumulatedPerks.m_flWeight; }
	float GetWeightCapacity() const { return m_pClass->m_flWeightCapacity; }
	bool IsAmmoFull() const;
	bool HasAccessoryUniqueFlag(EAUF flag) const {
		return (m_accumulatedPerks.m_iUniqueFlags & (int)flag);
	}

	//------------------------------------------------------------------
	// UI interaction
	//------------------------------------------------------------------
	bool WantsCursor() {
		return GetVelocity().Z > 1;
	}
};

