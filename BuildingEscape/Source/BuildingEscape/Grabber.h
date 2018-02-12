// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h" ///Needs to be in the header for intellisense to work in the CCP file
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	// How far ahead of the player we can reach in cm.
	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;

	bool bUseDebugLine = true;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-Cast and grab whats in reach
	void Grab();

	// Called when Grab key is released
	void Release();

	// Find (assumed) attached Physics Handle Component
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Draw Debug Line
	void DebugLine(FVector, FVector) const;

	// Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach();
	
	// Return current end of reach line
	FVector GetReachLineEnd();

	// Return current viewpoint location
	FVector GetReachLineStart();

};

