// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	/// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	/// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation); /// we defined the OUT macro at the top of this file which is just empty to																														   make sure that we can get the proper values as &out_ will make it into																														   something else than what we need it for!

	// TODO Log for output to test
	///UE_LOG(LogTemp, Warning, TEXT("Location: %s - Rotation is: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; /// need to use vector on rotation to make sure that the line follows the camera around, and not just																									forms to the world vector, but the camera vector of the pawn.
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(175, 238, 238), false, 0.0f, 0.0f, 5.0f);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); /// we're using simple trace so that we use the simple collision model over that of a complex one
															   /// We also make sure to ignore owner so we dont hit ourselves

	/// Line-Trace (AKA Ray-casting) out to reach distance
	FHitResult Hit; /// We use structs inside the paratheses if we havent converted them into a variable!
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters); 
	/// We want to use object type - remember to check object types in editor on its collision properties

	/// See what we hit
	AActor * ActorHit = Hit.GetActor();
	if (ActorHit) ///This is a check or it'll return null causing a game crash
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}
	

}

