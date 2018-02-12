// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "OpenDoor.generated.h"

/// This creates a new class!
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		// When we want to open the door
		FDoorEvent OnOpen; 

	UPROPERTY(BlueprintAssignable)
		// When we want to close the door
		FDoorEvent OnClose; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Angle") /// Allows us to use this variable in blueprint and edit it from the component in editor.
		// The angle which the door should open to
		float OpenAngle = 90.0f;

	// Get total mass in kg
	float GetTotalMassOfActorsOnPlate();

	/// Find (assumed) pressure plate
	void FindPressurePlate();

private:

	/// Important to initialise to nullptr
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr; 

	UPROPERTY(EditAnywhere)
		// *This value has to be LOWER than the total amount required!
		float TriggerMass = 0.0f;

		UPrimitiveComponent* Mass = nullptr;

		//UPROPERTY(EditAnywhere) // commented out so it doesnt appear in the editor property window - It automatically sets the pawn now due to the CCP BeginPlay method
		AActor* Owner = nullptr;

		
};
