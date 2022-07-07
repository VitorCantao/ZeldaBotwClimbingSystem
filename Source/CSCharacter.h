// Copyright Epic Games, Inc. All Rights Reserved.

// Base Character from UE5 Third Person Template

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSCharacter.generated.h"

class UMyCharacterMovementComponent;
UCLASS(config=Game)
class ACSCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UFUNCTION(BlueprintPure)
	FORCEINLINE UMyCharacterMovementComponent* GetCustomCharacterMovement() const { return MovementComponent; }
	
public:
	ACSCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

private:
	UPROPERTY()
	UMyCharacterMovementComponent* MovementComponent;

protected:
	void Climb();

	void CancelClimb();
	
	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	FRotationMatrix GetControlOrientationMatrix() const;

	virtual void Jump() override;
	
	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

