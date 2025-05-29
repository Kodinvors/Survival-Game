// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
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

	// Interaction
	UFUNCTION()
		void FindObject();

	// Player character mouse look
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamComp;

	// Creates Player stats, Health, Hunger, and Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;

	// Creates player resources wood, stone, berry
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	// Creates Resource Arrays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;
		
	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	// Creates hitDecal
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// Creates the Building Supplies Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> BuildingArray;

	// Creates the isBuilding boolean
	UPROPERTY()
		bool isBuilding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;

	UPROPERTY()
		ABuildingPart* spawnedPart;
	
	// create the playUI for the BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerWidget* playerUI;

	// SetHealth
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);
	
	// SetHunger
	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	// SetStamina
	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	// Decrease Stats
	UFUNCTION()
		void DecreaseStats();
	
	// GiveResource
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);

	// UpdateResources
	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	// SpawnBuilding
	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);

	// RotateBuilding
	UFUNCTION()
		void RotateBuilding();

};
