// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Components/LightComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "LightController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetColor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API ULightController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FSetColor OnPass;

	UPROPERTY(BlueprintAssignable)
		FSetColor OnDeny;

	UPROPERTY(BlueprintReadWrite, Category = "Trigger Volumes")
		bool FirstTriggerVolume = false;

	UPROPERTY(BlueprintReadWrite, Category = "Trigger Volumes")
		bool SecondTriggerVolume = false;
	//A pressure pads ARRAY number has to be the same as the Mass to Trigger
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Volumes")
		TArray<ATriggerVolume*> PressurePlates;

	UPROPERTY(BlueprintReadOnly, Category = "Array Spot")
		int32 ArrayNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LightObject")
		AActor* LightObject = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "LightObject")
		ULightComponent * LightComponent = nullptr;

private:

	int32 ArrayCount = 0;

	UPROPERTY(EditAnywhere, Category = "Trigger Volumes")
		//Total amount of mass required to trigger a pressure pad
		TArray<float> MassToTrigger;

	void FindLightComponent();
	void FindAndCreateEndCount();
	void CycleArraySpot();
	void FindLightObject();

	float GetTotalOverlappedMass();
	
	bool CompareMassTriggerWithPressurePlates();
	bool IsTriggerVolumeOverlapping();

};
