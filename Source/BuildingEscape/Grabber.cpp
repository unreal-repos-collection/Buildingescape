// copy right luca farci 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}




void UGrabber::SetupInputComponent()
{
	InputComponent =GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}	
}




// check for the physics handle component
void UGrabber::FindPhysicsHandle()
{	
	PhysicsHandle =GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle component on %s"),
		*GetOwner()->GetName());
	}
}




void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// if we hot something attach the physics handle
	if (ActorHit)
	{	if(!PhysicsHandle){return;}
		PhysicsHandle->GrabComponentAtLocation
			(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
			);
	}		
}



void UGrabber::Release()
{
	if(!PhysicsHandle){return;}
	PhysicsHandle->ReleaseComponent();
}




// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//if the physics handle is atttached
	if(!PhysicsHandle){return;} 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
	
	
}




FHitResult UGrabber::GetFirstPhysicsBodyInReach() const 
{
	//get player viewpoint
	FHitResult Hit;
	//ray-cast out to a certain distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")),false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
	OUT PlayerViewPointLocation,
	OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation; 
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
	OUT PlayerViewPointLocation,
	OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() *Reach;	
	
}
