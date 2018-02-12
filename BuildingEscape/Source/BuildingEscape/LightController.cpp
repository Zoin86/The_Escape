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
	GetLightComponent();
	// ...
	
}


// Called every frame
void ULightController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULightController::GetLightComponent()
{
	LightComponent = GetOwner()->FindComponentByClass<ULightComponent>();
	if (!LightComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Light Component: NOT FOUND!"))
			
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Light Component: FOUND! "));
	}
}

void ULightController::SetLightColor()
{
	OnSetLightColorRequest.Broadcast();
}

void ULightController::GetPressurePlates()
{
	auto TotalMass = TriggerDoor->GetTotalMassOfActorsOnPlate();
}


	