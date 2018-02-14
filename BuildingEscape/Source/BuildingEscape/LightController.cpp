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
	FindLightObject();
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
			OnPass.Broadcast();
		}
		else
		{
			OnDeny.Broadcast();
		}
	}
	else
	{
		OnDeny.Broadcast();
	}
	CycleArraySpot();
}

void ULightController::FindLightObject()
{
	//make sure not to use !LightObject - will cause a crash - instead do below
	if (LightObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: No light Object Connected!"), *GetOwner()->GetName())
		return;
	}
	else
	{
		FindLightComponent();
		//Make sure NOT to use *LightObject->GetOwner->GetName() - Will cause game crash
		UE_LOG(LogTemp, Warning, TEXT("%s: and Light Object Connected: %s"), *GetOwner()->GetName(), *LightObject->GetName())
	}

}

void ULightController::FindLightComponent()
{
	LightComponent = LightObject->FindComponentByClass<ULightComponent>();

	if (LightComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is MISSING a Light Component!"), *LightObject->GetName())	
		return;
	}
}

void ULightController::FindAndCreateEndCount()
{
	ArrayCount = PressurePlates.Num();
	if (PressurePlates.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pressure Plates attached to %s "), *GetOwner()->GetName())		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s: Pressure Plate Count: %i "), *GetOwner()->GetName(), ArrayCount)
}

// Returns true if Trigger Volume is overlapping with Actor
bool ULightController::IsTriggerVolumeOverlapping()
{
	if(PressurePlates.Num() < 1) {return false;} /// Don't use !PressurePaltes as it is an array - will cause game crash - check against its amount of objects instead!
	if(PressurePlates[ArrayNumber] == nullptr){	return false; }
	
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
}

bool ULightController::CompareMassTriggerWithPressurePlates()
{
	if (MassToTrigger.Num() < 1){ return false; }

	int32 ArraySpotCount = ArrayNumber + 1;

	if (ArrayCount >= 1)
	{
		if (GetTotalOverlappedMass() >= MassToTrigger[ArrayNumber])
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
		UE_LOG(LogTemp, Warning, TEXT("%s: Got nothing to Compare!"), *GetOwner()->GetName())
			
		return false;
	}

}

// Gets total mass, and puts it into array
float ULightController::GetTotalOverlappedMass()
{
	float TotalMass = 0.0f;

	if (ArrayCount >= 1)
	{
		TArray<float> PressurePlateOverlappingMass; /// Don't put in header file as this will never be reset properly, instead make it a local scoped array

		TArray<AActor*> OverlappingActors;
		PressurePlates[ArrayNumber]->GetOverlappingActors(OUT OverlappingActors);
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume: %s - Overlapping actor amount: %i"), *PressurePlates[ArrayNumber]->GetName(), OverlappingActors.Num())

		for (const auto* Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->CalculateMass(NAME_None); /// Get Overlapping Mass
			PressurePlateOverlappingMass.Add(TotalMass); /// add it to PressurePlateOverlappingMass TArray
		}
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume: %s - PressurePlateMass Objects: %i"), *PressurePlates[ArrayNumber]->GetName(), PressurePlateOverlappingMass.Num())
		
		return TotalMass;
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
}
