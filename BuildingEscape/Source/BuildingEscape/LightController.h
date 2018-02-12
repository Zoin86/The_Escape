// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/LightComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "OpenDoor.h"
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
		FSetColor OnSetLightColorRequest;

private:



	UOpenDoor * TriggerDoor = nullptr;

	ULightComponent * LightComponent = nullptr;

	UPROPERTY(EditAnywhere)
		TArray<ATriggerVolume*> PressurePlatesMass;

	void GetLightComponent();

	void SetLightColor();

	void GetPressurePlates();


		
	
};
