// Copyright Mark Davidson 2021

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// For readability - markup out params
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	CurrentYaw = InitialYaw = GetOwner()->GetActorRotation().Yaw;
	DoorOpenAngle += InitialYaw;
	CheckPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::CheckPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Component %s does not have a PressurePlate"), *GetOwner()->GetName() )
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing AudioComponent"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (TotalMassOnPad() > PadActivationMass)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = CurrentTime;
	}
	else
	{
		if (CurrentTime > (DoorLastOpened + DoorCloseDelay))
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = FMath::FInterpTo(CurrentYaw, DoorOpenAngle, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRotation(DoorRotation);

	if (AudioComponent && !bOpenSoundPlayed)
	{
		AudioComponent->Play();
		bCloseSoundPlayed = false;
		bOpenSoundPlayed = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(DoorRotation);

	if (AudioComponent && !bCloseSoundPlayed)
	{
		AudioComponent->Play();
		bCloseSoundPlayed = true;
		bOpenSoundPlayed = false;
	}
}

float UOpenDoor::TotalMassOnPad() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors and add their masses
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) return TotalMass;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for(AActor* ActorOnPad : OverlappingActors)
	{
		TotalMass += ActorOnPad->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}