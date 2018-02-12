// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	/// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	/// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached  Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	FString PawnName = GetOwner()->GetName();
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		///Finds the default pawn since its looking through the component tree of the owner.
		UE_LOG(LogTemp, Error, TEXT("%s is missing Physics Handle Component!"), *PawnName);
	}
}

/// Look for attached InputComponent (only appears at runtime)
void UGrabber::SetupInputComponent()
{
	FString PawnName = GetOwner()->GetName();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); ///Grab needs to be spelled the exact same as it has been set in the editor
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing Input Component!"), *PawnName);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed!"));

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // Get's the mesh in our case
	auto ActorHit = HitResult.GetActor();
	/// If we hit something then attached a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)	{ return; }

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, /// Bone name - No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

// Release component from physics handle
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released!"));
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)	{ return; }

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());	/// move object we're holding each frame
	}

}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation); /// We defined OUT to nothing so we dont get with the memory address of &out_
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; /// need to use vector on rotation to get the pawn vector and not world vector
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation; 
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	if (bUseDebugLine)
	{
		DebugLine(GetReachLineStart(), GetReachLineEnd());
	}
	
	/// Line-Trace (AKA Ray-casting) out to reach distance
	FHitResult HitResult;
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); /// we're using simple collisionbox trace so that we use the simple collision model over that of a complex one
																		   /// We also make sure to ignore owner so we dont hit ourselves
	/// Set the physics channel we want to interact with
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters); 	
	/// We want to use object type - remember to check object types in editor on its collision properties

	return HitResult;
}

void UGrabber::DebugLine(FVector ViewPointLocation, FVector EndLine) const
{
	DrawDebugLine(GetWorld(), ViewPointLocation, EndLine, FColor(175, 238, 238), false, 0.0f, 0.0f, 5.0f);
}
