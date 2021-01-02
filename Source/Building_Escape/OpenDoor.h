// Copyright Mark Davidson 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float DoorOpenAngle = 90.0f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = NULL;

	UPROPERTY(EditAnywhere)
	AActor * ActorThatOpens = NULL;

	float DoorLastOpened = 0.0f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.0f;
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.0f;
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 4.0f;
};
