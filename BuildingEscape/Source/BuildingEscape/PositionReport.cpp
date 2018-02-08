// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();
	
	// pointer method - Using arrow pointer due to the star on *UActorComponent::GetOwner()
	FString ObjectName = GetOwner()->GetName(); // Gets name of object component is attached to
	FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToString(); // Gets location and converts it to string from which the component is attached to
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos); // remember to use Star infront of ObjectName when using variables which comes from pointer methods
	//Outputs name, two words, and object position
	
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

