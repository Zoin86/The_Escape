// Fill out your copyright notice in the Description page of Project Settings.

#include "LightController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


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

	// ...
}

void ULightController::FindLightComponent()
{
	LightComponent = GetOwner()->FindComponentByClass<ULightComponent>();
	if (!LightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Light Component: MISSING!"))
			
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Light Component: FOUND! "));
	}
}

void ULightController::SetStatueLightColor()
{
	OnSetLightColorRequest.Broadcast();
}

void ULightController::FindAndCreateEndCount()
{
	EndCount = PressurePlates.Num();
	UE_LOG(LogTemp, Warning, TEXT("Pressure Plate Count: %i "), EndCount);
	return;
}

// Gets total mass, and puts it into array
void ULightController::GetTotalMassAndInsertIntoOverLappingMassArray()
{
	if (EndCount >= 1)
	{
		for (auto& Actor : PressurePlates)// iterate through pressure plates
		{
			float TotalMass = 0.0f;

			for (int32 StartCount = 0; StartCount < EndCount; StartCount++)
			{
				TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->CalculateMass(NAME_None); /// Get Overlapping Mass
				PressurePlateOverLappingMass.Insert(float(TotalMass), StartCount); /// add it to PressurePlateOverlappingMass TArray
			}
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No pressure Plates Found!"));
	}
	return;
}
