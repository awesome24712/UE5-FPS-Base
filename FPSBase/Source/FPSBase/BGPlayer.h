// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Util/JSON/PropertyBinder.h"
#include "PODT/DamageInfo.h"
#include "Systems/PermissionSystem.h"
#include "Systems/FactionSystem.h"
//#include "Profiles/WeaponSystemProfile.h"
#include "BGPlayer.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

class KitAccessoryProfile;
class WeaponDefProfile;

class ABGPlayer;
ABGPlayer* GetLocalPlayer();

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class ABGPlayer : public ACharacter, public IJsonBindable, public FIDamageable
{
	GENERATED_BODY()

	friend class Faction;

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
	const PlayerClass* m_pClass;
	WeaponDefProfile* m_pPrimaryWeaponSelection;
	WeaponDefProfile* m_pSecondaryWeaponSelection;
	WeaponDefProfile* m_pTertiaryWeaponSelection;

	KitAccessoryProfile* m_pPrimaryWeaponPerk;
	KitAccessoryProfile* m_pSecondaryWeaponPerk;

	KitAccessoryProfile* m_pClassSpecificPerk;
	KitAccessoryProfile* m_pPrimaryClassPerk;
	KitAccessoryProfile* m_pSecondaryClassPerk;

	uint8 m_iAmmoIncrement; //5 shots per increment, adds weight too

	//per-life stats
	uint8 m_iHealth;
	uint8 m_iStamina;
	uint8 m_iBandages;

public:
	ABGPlayer();

protected:
	virtual void BeginPlay();

public:

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	void CustomJump();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//WeaponSystemProfile* GetActiveWeaponSystemProfile() const { return m_pActiveWeaponSystemProfile; }

	Faction* GetFaction() const;
	FString GetPlayerName() const { return ""; }
};

