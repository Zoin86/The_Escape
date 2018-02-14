// Fill out your copyright notice in the Description page of Project Settings.

#include "LightController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
ULightController::ULightController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightController::BeginPlay()
{
	Super::BeginPlay();
	FindLightComponent();
	FindAndCreateEndCount();
}


// Called every frame
void ULightController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (IsTriggerVolumeOverlapping())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsTriggerVolumeOverlapping[0]: working!"))

		if (CompareMassTriggerWithPressurePlates())
		{
			SetStatueLightColor();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IsTriggerVolumeOverlapping: NOT WORKING"))
		
		return;
	}

	if (IsTriggerVolumeOverlapping())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsTriggerVolumeOverlapping[1]: working!"))

		if (CompareMassTriggerWithPressurePlates())
		{
			SetStatueLightColor();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IsTriggerVolumeOverlapping: NOT WORKING"))
		
			return;
	}*/
	
	//Activate to in case you need to check
	//IsArrayValid(); 
	
	debug = CompareMassTriggerWithPressurePlates();
	UE_LOG(LogTemp, Warning, TEXT("Returns: %i"), debug)
	CycleArraySpot();
}

void ULightController::FindLightComponent()
{
	LightComponent = GetOwner()->FindComponentByClass<ULightComponent>();
	if (!LightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Light Component: MISSING!"))
			
		return;
	}
}

void ULightController::SetStatueLightColor()
{
	OnSetLightColorRequest.Broadcast();
}

void ULightController::FindAndCreateEndCount()
{
	ArrayEndCount = PressurePlates.Num();
	if (PressurePlates.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pressure Plates attached to Light Statue! "))
		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Pressure Plate Count: %i "), ArrayEndCount)
	
	return;
}

// Returns true if Trigger Volume is overlapping with Actor
bool ULightController::IsTriggerVolumeOverlapping()
{
	if (PressurePlates.Num() < 1)
	{
		return false;
	}


	//TODO Need to move OverlappingActors Array out of this method
	TArray<AActor*> OverlappingActors;
	PressurePlates[ArraySpot]->GetOverlappingActors(OUT OverlappingActors);

	auto OverLappingActorCount = OverlappingActors.Num();
	auto PlateName = PressurePlates[ArraySpot]->GetOwner()->GetName();
		
	if (OverlappingActors.Num() >= 1) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressure plate %s has %i overlapping! "), *PlateName, OverLappingActorCount);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing is overlapping"))
	}
	return false;
}

bool ULightController::CompareMassTriggerWithPressurePlates()
{
	int32 ArraySpotCount = ArraySpot + 1;

	if (ArrayEndCount >= 1)
	{
		GetTotalMassAndInsertIntoOverLappingMassArray();

		if (MassTrigger[ArraySpot] >= PressurePlateOverlappingMass[ArraySpot])
		{
			UE_LOG(LogTemp, Warning, TEXT("Array Spot Counter: %i "), ArraySpotCount)
			
			return true;
		}
		else
		{
			float MassTriggerSpot = MassTrigger[ArraySpot];
			UE_LOG(LogTemp, Warning, TEXT("You should not see this!"))
			UE_LOG(LogTemp, Warning, TEXT("MassTrigger[%i]: %f"), ArraySpot, MassTriggerSpot)
			UE_LOG(LogTemp, Warning, TEXT("MassTrigger Objects: %i"), MassTrigger.Num())
			UE_LOG(LogTemp, Warning, TEXT("Pressure Plate Overlap Mass: %f - Objects: %i"), PressurePlateOverlappingMass[ArraySpot], PressurePlateOverlappingMass.Num())

			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing to Compare!"))

		return false;
	}

	return false;
}

// Gets total mass, and puts it into array
void ULightController::GetTotalMassAndInsertIntoOverLappingMassArray()
{
	float TotalMass = 0.0f;

	if (ArrayEndCount >= 1)
	{
		if (!PressurePlates[ArraySpot]){ UE_LOG(LogTemp, Error, TEXT("Actor Pointing to nullptr"))	return;	}


		//TODO Need to change PressurePlate Array to Overlapping array after Overlapping array has been moved!
		TotalMass += PressurePlates[ArraySpot]->FindComponentByClass<UPrimitiveComponent>()->CalculateMass(NAME_None); /// Get Overlapping Mass
		PressurePlateOverlappingMass.Insert(float(TotalMass), ArraySpot); /// add it to PressurePlateOverlappingMass TArray
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No pressure Plates Found!"))
	}

}

void ULightController::CycleArraySpot()
{
	if (ArraySpot >= 1)
	{
		ArraySpot = -1;
	}

	ArraySpot++;
}

void ULightController::IsArrayValid()
{
	if (bool IsArrayValid0 = PressurePlates.IsValidIndex(ArraySpot))
	{
		UE_LOG(LogTemp, Warning, TEXT("Array: PressurePlates is valid"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array: PressurePlates is NOT valid"))
	}

	if (bool IsArrayValid1 = MassTrigger.IsValidIndex(ArraySpot))
	{
		UE_LOG(LogTemp, Warning, TEXT("Array: MassTrigger is valid"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array: MassTrigger is NOT valid"))
	}

	if (bool IsArrayValid2 = PressurePlateOverlappingMass.IsValidIndex(ArraySpot))
	{
		UE_LOG(LogTemp, Warning, TEXT("Array: PressurePlateOverlappingMass is valid"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array: PressurePlateOverlappingMass is NOT valid"))
	}
}
