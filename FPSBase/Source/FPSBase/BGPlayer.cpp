// Copyright Epic Games, Inc. All Rights Reserved.

#include "BGPlayer.h"
#include "FPSBaseProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Components/MovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Util/FileIO/Logger.h"
#include "Systems/FactionSystem.h"
#include "UI/UIConductor.h"
#include "CommonHeaders.h"

static ABGPlayer* g_pLocalPlayer;
ABGPlayer* GetLocalPlayer() {
	return g_pLocalPlayer;
}


//////////////////////////////////////////////////////////////////////////
// ABGPlayer

ABGPlayer::ABGPlayer()
{
	g_pLocalPlayer = this;

	//PC().GetHUD()->ShowHUD();

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	SetupFPV_MeshComponent();

	
	OnActorHit.AddUniqueDynamic(this, &ABGPlayer::OnPhysicsHit);


	JT_START_BINDING_UCLASS("BGPlayer", ABGPlayer);
	JT_BIND_DOUBLE(m_jumpSpeed, "jumpSpeed", false);
	JT_BIND_DOUBLE(m_runSpeed, "runSpeed", false);
	JT_FINISH_BINDING();
}

void ABGPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	LoadBindingsFromJson("player_movement.json");
	GetCharacterMovement()->JumpZVelocity = m_jumpSpeed;
	GetCharacterMovement()->MaxWalkSpeed = m_runSpeed;

	if (DataSystem::g_weapons.Num()) {
		m_pPrimaryWeaponSelection = DataSystem::g_weapons[0];
	}

	SetActorTickEnabled(true);

	Spawn(true);
}

//////////////////////////////////////////////////////////////////////////// Input

void ABGPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABGPlayer::CustomJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ABGPlayer::OnPrimaryAction);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &ABGPlayer::OnSecondaryAction);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABGPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABGPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}

void ABGPlayer::SetupFPV_MeshComponent() {
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void ABGPlayer::OnPrimaryAction()
{
	Msg("Primary attack");
}

void ABGPlayer::OnSecondaryAction() {
	Msg("Secondary attack");
}



void ABGPlayer::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABGPlayer::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABGPlayer::CustomJump()
{
	Msg("Jump!");
	Jump();
}

FVector ABGPlayer::GetAimDirection() const {
	const FRotator rotation = GetBaseAimRotation();
	return rotation.RotateVector(FVector::ForwardVector);
}


//------------------------------------------------------------------
// player management functions
//------------------------------------------------------------------
void ABGPlayer::SwapToPlayerClass(const PlayerClassProfile* pClass, bool bForceNow) {


	if (bForceNow) {
		Spawn(true);
	}
}

//------------------------------------------------------------------
// per-frame functions
//------------------------------------------------------------------
void ABGPlayer::Tick(float deltaSeconds) {
	GetCharacterMovement()->MaxWalkSpeed = GetMaxSpeed();
	
	m_iStamina += 1;
	if (m_iStamina > 100) m_iStamina = 100;

	
}

//------------------------------------------------------------------
// per-life functions
//------------------------------------------------------------------
void ABGPlayer::Spawn(bool bForce) {
	//do nothing if alive and we're not forcing
	if (IsAlive() && !bForce) {
		return;
	}


	//Refill/clear per-life variables
	SetHealth(GetMaxHealth());
	m_iStamina = 100;
	m_iRallyFlags = 0;
	m_iBandages = 0; //assume 0, will give later if needed
	m_iHealthToLose = 0;
	m_iHealthLostToBleeding = 0;

	if (m_bLoadoutChangePending) {
		m_loadout.LoadOntoPlayer(this);
		AccumulatePerks();
		AccumulateAVs();
		m_bLoadoutChangePending = false;
	}

	GiveSpawnWeapons();
	GiveAmmoFull();
	SetCosmetics();


	//TODO find spawn location
	FVector spawnLocation = GetActorLocation();
	FQuat spawnRotation = GetActorQuat();

	//Teleport player to spawn
	SetActorLocation(spawnLocation, false, NULL, ETeleportType::ResetPhysics);
	SetActorRotation(spawnRotation, ETeleportType::ResetPhysics);

	//TODO player spawn sound

}



void ABGPlayer::GiveSpawnWeapons() {

}

void ABGPlayer::GiveAmmoFull() {
	if (IsAmmoFull()) {
		return;
	}
}

void ABGPlayer::SetCosmetics() {

}

//------------------------------------------------------------------
// FIDamageable overrides
//------------------------------------------------------------------
void ABGPlayer::FilterDamage(FDamageInfo& di) {

}
void ABGPlayer::OnTakeDamage(const FDamageInfo& di) {
	if (!HasAccessoryUniqueFlag(EAUF::DAMAGE_NOT_EFFECTS)) {
		DrainStamina(di.GetDamage());
	}
	
}
void ABGPlayer::OnDeath(const FDamageInfo& di) {

}
void ABGPlayer::OnTakeDamageDeathBlocked(const FDamageInfo& di) {

}
void ABGPlayer::OnHealed(const FDamageInfo& di) {

}
void ABGPlayer::OnStunned(const FDamageInfo& di) {

}

//------------------------------------------------------------------
// movement functions
//------------------------------------------------------------------
float ABGPlayer::GetMaxSpeed() {
	float staminaMultiplier = m_iStamina / 100.f;

	//TODO walk, crouch, reload, officer buff

	return 
		(BASE_SPEED_ALL_CLASSES * staminaMultiplier)
		+ m_accumulatedAVs.m_iSpeed * SPEED_INCREMENT_PER_AV;
}

void ABGPlayer::DrainStamina(int drain) {
	drain *= m_accumulatedPerks.m_flMeleeStaminaDrainMultiplier;

	m_iStamina -= drain;
}

void ABGPlayer::OnPhysicsHit(AActor* self, AActor* other, FVector normalImpulse, const FHitResult& hit) {
	FVector v = GetVelocity();
 	Msg(UIHelpers::VectorToString(v));
	if (v.SquaredLength() > (50 * 50)) {
		//build damage info
		FDamageInfo di = FDamageInfo(v.Length() / 2, this, EDamageType::DMG_GRAVITY);
		FIDamageable::TakeDamage(di);
	}
}

//------------------------------------------------------------------
// const getters
//------------------------------------------------------------------
Faction* ABGPlayer::GetFaction() const {
	return FactionFromTeamNumber(m_iTeam);
}

bool ABGPlayer::IsAmmoFull() const {
	return false;
}