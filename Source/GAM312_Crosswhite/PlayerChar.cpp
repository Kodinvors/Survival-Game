// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initial setup of camera component. 
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// Attaching camera to the character mesh and head bone.
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// Share rotation with controller
	PlayerCamComp->bUsePawnControlRotation = true;

	// Instantiate Arrays
	BuildingArray.SetNum(3);
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));



}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// Create a timer, rather than call on delta time.
	FTimerHandle StatsTimerHandle;
	// Set timer to engage every two seconds.
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update health, hunger, and stamina on tick.
	playerUI->UpdateBars(Health, Hunger, Stamina);

	// While building set building spawn location.
	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);

}

// Player input: Move Forward
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

// Player input: Move Right
void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

// Player input: Press Jump
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

// Player input: Release Jump
void APlayerChar::StopJump()
{
	bPressedJump = false;
}

// Player input: Left Click Interaction
void APlayerChar::FindObject()
{
	// Hit identification and range
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	// Enable resource collection while not in building mode
	if (!isBuilding)
	{
		// Project line tracing
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			if (Stamina > 5.0f)
			{

				if (HitResource)
				{
					// Identify resource hit and quantity collected
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					// Set remaining resource amount for source
					HitResource->totalResource = HitResource->totalResource - resourceValue;
					
					// Reduce Stamina on hit
					SetStamina(-5.0f);

					// Determine if the resource has been depleted
					if (HitResource->totalResource > resourceValue)
					{
						// Grants player appropriate resource
						GiveResource(resourceValue, hitName);

						// Dev resource collection confirmation
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						// Apply decal at hit location
						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);
					}
					else
					{
						// Remove mesh on resource depletion
						HitResource->Destroy();

						// Dev resource depleted confirmation
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}
		}
	}

	else
	{
		isBuilding = false;
	}
}

// SetHealth increase, not to exceed 100.
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}

// SetHunger increase, not to exceed 100.
void APlayerChar::SetHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}

// SetStamina increase, not to exceed 100.
void APlayerChar::SetStamina(float amount)
{
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}

// Decrease player Stats on call
void APlayerChar::DecreaseStats()
{
	if (Hunger > 0)
	{
		SetHunger(-1.0f);

		// Increase stamina while hunger is above 0
		SetStamina(10.0f);
	}

	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}

// Increase player owned wood, stone, and berry on resource collection
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

// Update Player resource quantities on building creation
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}

// Spawns the players chosen build item
void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	// Prevents two or more building parts from being placed at one time
	if (!isBuilding)
	{
		// Checks that a build piece has been created to spawn
		if (BuildingArray[buildingID] >= 1)
		{
			// Sets build mode to true and determines player location, and sets item to be placed at set distance
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			// Reduces available building part count on use
			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			// sets part location to be spawned
			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false;
	}
}

// Rotates building part on input command
void APlayerChar::RotateBuilding()
{
	if (isBuilding)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

