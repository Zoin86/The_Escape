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

	if (IsTriggerVolumeOverlapping())
	{
		if (CompareMassTriggerWithPressurePlates())
		{
			SetStatueLightColor();
		}
	}

	//Activate to in case you need to check
	//IsArrayValid(); 
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
	ArrayCount = PressurePlates.Num();
	if (PressurePlates.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pressure Plates attached to Light Statue! "))
		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Pressure Plate Count: %i "), ArrayCount)
	
	return;
}

// Returns true if Trigger Volume is overlapping with Actor
bool ULightController::IsTriggerVolumeOverlapping()
{
	if (PressurePlates.Num() < 1)
	{
		return false;
	}

	TArray<AActor*> OverlappingActors;
	PressurePlates[ArrayNumber]->GetOverlappingActors(OUT OverlappingActors);

	auto OverLappingActorCount = OverlappingActors.Num();
	auto PlateName = PressurePlates[ArrayNumber]->GetName(); /// Getting owner on array spots will cause a game crash !
		
	if (OverlappingActors.Num() >= 1) 
	{
		return true;
	}
	else
	{
		return false;
	}
	//return false;
}

bool ULightController::CompareMassTriggerWithPressurePlates()
{
	int32 ArraySpotCount = ArrayNumber + 1;

	if (ArrayCount >= 1)
	{
		GetTotalMassAndInsertIntoOverLappingMassArray();

		if (PressurePlateOverlappingMass[ArrayNumber] >= MassTrigger[ArrayNumber])
		{	
			return true;
		}
		else
		{
				
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing to Compare!"))
			
		return false;
	}

}

// Gets total mass, and puts it into array
void ULightController::GetTotalMassAndInsertIntoOverLappingMassArray()
{
	float TotalMass = 0.0f;

	if (ArrayCount >= 1)
	{
		if (!PressurePlates[ArrayNumber]){ UE_LOG(LogTemp, Error, TEXT("Actor Pointing to nullptr"))	return;	}

		if (PressurePlateOverlappingMass.Num() >= ArrayCount) /// need to empty the array so that it doesnt keep adding new objects to it.
		{
			PressurePlateOverlappingMass.Empty();
		}

		TArray<AActor*> OverlappingActors;
		PressurePlates[ArrayNumber]->GetOverlappingActors(OUT OverlappingActors);

		for (const auto* Actor : OverlappingActors)
		{
			TotalMass += OverlappingActors[ArrayNumber]->FindComponentByClass<UPrimitiveComponent>()->CalculateMass(NAME_None); /// Get Overlapping Mass
			PressurePlateOverlappingMass.Insert(float(TotalMass), ArrayNumber); /// add it to PressurePlateOverlappingMass TArray
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No pressure Plates Found!"))
	}

}

void ULightController::CycleArraySpot()
{
	if (ArrayNumber >= ArrayCount - 1) /// Array Count needs to be one less or it won't be accurate as ArrayNumber starts at 0 where ArrayCount starts at 1
	{
		ArrayNumber = -1;
	}

	ArrayNumber++;
}

void ULightController::IsArrayValid()
{
	if (bool IsArrayValid0 = PressurePlates.IsValidIndex(ArrayNumber))
	{
		UE_LOG(LogTemp, Warning, TEXT("Array: PressurePlates is valid"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array: PressurePlates is NOT valid"))
	}

	if (bool IsArrayValid1 = MassTrigger.IsValidIndex(ArrayNumber))
	{
		UE_LOG(LogTemp, Warning, TEXT("Array: MassTrigger is valid"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array: MassTrigger is NOT valid"))
	}

	if (bool IsArrayValid2 = PressurePlateOverlappingMass.IsValidIndex(ArrayNumber))
	{
		UE_LOG(LogTemp, Warning, TEXT("Array: PressurePlateOverlappingMass is valid"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array: PressurePlateOverlappingMass is NOT valid"))
	}
}
