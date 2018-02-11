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

	// ...
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

	if (PhysicsHandle)
	{
		// Physics Handle Found
	}
	else
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
		UE_LOG(LogTemp, Warning, TEXT("%s Input Component found!"), *PawnName);
		/// Bind Input Action
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
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	/// If we hit something then attached a physics handle
	if (ActorHit)
	{
		// TODO attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab, /// 
			NAME_None, /// Bone name - Though we aren't actually looking for a bone name, so this is none
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}

			


}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released!"));
	
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		// move object we're holding each frame
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		
	// otherwise 
		// do nothing

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation); /// We defined OUT to nothing so we dont get with the memory address of &out_

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach; /// need to use vector on rotation to get the pawn vector and not world vector
	if (bUseDebugLine)
	{
		DebugLine(PlayerViewPointLocation, LineTraceEnd);
	}

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); /// we're using simple trace so that we use the simple collision model over that of a complex one
																			   /// We also make sure to ignore owner so we dont hit ourselves
	/// Line-Trace (AKA Ray-casting) out to reach distance
	FHitResult Hit;
	/// Set the physics channel we want to interact with
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters); 
	/// We want to use object type - remember to check object types in editor on its collision properties

	/// See what we hit
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit) ///This is a check or it'll return null causing a game crash
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
	
}

void UGrabber::DebugLine(FVector ViewPointLocation, FVector EndLine) const
{
	DrawDebugLine(GetWorld(), ViewPointLocation, EndLine, FColor(175, 238, 238), false, 0.0f, 0.0f, 5.0f);
}
