// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "OpenDoor.generated.h"


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

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY(EditAnywhere) // using a marcro to create properties in our component in the editor
		float OpenAngle = 90.0f;

	UPROPERTY(EditAnywhere)
		float CloseAngle = 0.0f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr; /// Important to initialise to nullptr

	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 1.0f;

	UPROPERTY(EditAnywhere)
		float MassToTriggerPressurePlate = 0.0f;

	/// Find (assumed) pressure plate
	void FindPressurePlate();

	/// Used to calculate when to close in OpenDoor.ccp
		float LastDoorOpenTime;
		
		UPrimitiveComponent* Mass = nullptr;

		//UPROPERTY(EditAnywhere) // commented out so it doesnt appear in the editor property window - It automatically sets the pawn now due to the CCP BeginPlay method
		AActor* Owner = nullptr;

		// Get total mass in kg
		float GetTotalMassOfActorsOnPlate();

};
