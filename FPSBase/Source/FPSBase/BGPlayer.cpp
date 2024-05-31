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

static ABGPlayer* g_pLocalPlayer;
ABGPlayer* GetLocalPlayer() {
	return g_pLocalPlayer;
}


//////////////////////////////////////////////////////////////////////////
// ABGPlayer

ABGPlayer::ABGPlayer()
{
	g_pLocalPlayer = this;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	JT_START_BINDING_UCLASS("Hunter", ABGPlayer);
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

	JsonTreeHandle::CreateFromDirectory("");
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

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABGPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABGPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}

void ABGPlayer::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
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
	Jump();
}

Faction* ABGPlayer::GetFaction() const {
	return FactionFromTeamNumber(m_iTeam);
}