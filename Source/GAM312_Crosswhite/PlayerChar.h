// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_CROSSWHITE_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Move player character forwards or backwards
	UFUNCTION()
		void MoveForward(float axisValue);

	// Move player character left or right
	UFUNCTION()
		void MoveRight(float axisValue);

	// Begin jump
	UFUNCTION()
		void StartJump();

	// End jump
	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void FindObject();

	// Player character mouse look
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamComp;

};
