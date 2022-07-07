// Copyright Epic Games, Inc. All Rights Reserved.

// Base Character from UE5 Third Person Template

#include "CSCharacter.h"

#include "Public/MyCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACSCharacter

ACSCharacter::ACSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	TurnRateGamepad = 50.f;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MovementComponent = Cast<UMyCharacterMovementComponent>(GetCharacterMovement()); // <--
}

void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &ACSCharacter::Climb);
	PlayerInputComponent->BindAction("Cancel Climb", IE_Released, this, &ACSCharacter::CancelClimb);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACSCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACSCharacter::LookUpAtRate);
}

void ACSCharacter::Climb()
{
	MovementComponent->TryClimbing();
}

void ACSCharacter::CancelClimb()
{
	MovementComponent->CancelClimbing();
}

void ACSCharacter::MoveForward(float Value)
{
	if (Controller == nullptr || Value == 0.0f)
	{
		return;
	}

	FVector Direction;
	
	if (MovementComponent->IsClimbing())
	{
		Direction = FVector::CrossProduct(MovementComponent->GetClimbSurfaceNormal(), -GetActorRightVector());
	}
	else
	{
		Direction = GetControlOrientationMatrix().GetUnitAxis(EAxis::X);
	}
	
	AddMovementInput(Direction, Value);
}

void ACSCharacter::MoveRight(float Value)
{
	if (Controller == nullptr || Value == 0.0f)
	{
		return;
	}

	FVector Direction;
	if (MovementComponent->IsClimbing())
	{
		Direction = FVector::CrossProduct(MovementComponent->GetClimbSurfaceNormal(), GetActorUpVector());
	}
	else
	{
		Direction = GetControlOrientationMatrix().GetUnitAxis(EAxis::Y);
	}
	
	AddMovementInput(Direction, Value);
}

FRotationMatrix ACSCharacter::GetControlOrientationMatrix() const
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	return FRotationMatrix(YawRotation);
}

void ACSCharacter::Jump()
{
	if (MovementComponent->IsClimbing())
	{
		MovementComponent->TryClimbDashing();
	}
	else
	{
		Super::Jump();
	}
}

void ACSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}
