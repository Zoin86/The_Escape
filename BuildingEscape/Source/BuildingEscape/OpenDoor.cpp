// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindPressurePlate();
	Owner = GetOwner(); // find owning actor

}

/// Look for attached  Physics Handle
void UOpenDoor::FindPressurePlate()
{
	FString ActorName = GetOwner()->GetName();

	if (PressurePlate == nullptr)
	{
		///Finds the default pawn since its looking through the component tree of the owner.
		UE_LOG(LogTemp, Error, TEXT("Open Door Component of: %s Has no PRESSURE PLATE attached!"), *ActorName);
	}
}

void UOpenDoor::CloseDoor()
{
	// Set rotation back to Closed angle
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // Compares the Total mass on actors inside the trigger volume
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{

	float TotalMass = 0.0f;

	if (!PressurePlate)	{ return TotalMass; }
	// Find all overlapping actors
	TArray<AActor*> OverlappingActors; /// like a list in C#
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	//auto ComponentMass = Mass->GetMassScale(NAME_None);

	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->CalculateMass(NAME_None);
		UE_LOG(LogTemp, Warning, TEXT("Pressure plate holds: %s"), *Actor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Total Mass on Pressure Plate: %fkg"), TotalMass);
	}
	
	return TotalMass; //
}
