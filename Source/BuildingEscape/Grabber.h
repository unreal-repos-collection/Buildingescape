// copy right luca farci 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// line reach
	UPROPERTY(EditAnywhere)
	float Reach = 150.f;
	
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();	
	void FindPhysicsHandle();
	void SetupInputComponent();

	//return the first actor in reach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	// return the line trace end
	FVector GetPlayersReach() const ;

	// get players position in the world
	FVector GetPlayersWorldPos() const;
};
