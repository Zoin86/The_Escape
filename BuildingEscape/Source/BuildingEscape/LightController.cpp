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
		if (GetTotalMass() >= MassTrigger[ArrayNumber])
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
float ULightController::GetTotalMass()
{
	float TotalMass = 0.0f;

	if (ArrayCount >= 1)
	{
		if (!PressurePlates[ArrayNumber]){ UE_LOG(LogTemp, Error, TEXT("Actor Pointing to nullptr"))	return TotalMass;	}

		TArray<float> PressurePlateOverlappingMass;

		TArray<AActor*> OverlappingActors;
		PressurePlates[ArrayNumber]->GetOverlappingActors(OUT OverlappingActors);
		UE_LOG(LogTemp, Error, TEXT("Overlapping actor amount: %i"), OverlappingActors.Num())

		for (const auto* Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->CalculateMass(NAME_None); /// Get Overlapping Mass
			PressurePlateOverlappingMass.Add(TotalMass); /// add it to PressurePlateOverlappingMass TArray
		}
		UE_LOG(LogTemp, Error, TEXT("PressurePlateMass Objects: %i"), PressurePlateOverlappingMass.Num())
		
		return TotalMass;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No pressure Plates Found!"))
	}
	return TotalMass;
}

void ULightController::CycleArraySpot()
{
	if (ArrayNumber >= ArrayCount - 1) /// Array Count needs to be one less or it won't be accurate as ArrayNumber starts at 0 where ArrayCount starts at 1
	{
		ArrayNumber = -1;
	}
	ArrayNumber++;
	UE_LOG(LogTemp, Error, TEXT("Array Number: %i"), ArrayNumber)

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
}
