#include "ACrouchZone.h"

#include "Components/BoxComponent.h"
#include "FPSPlayer_BaseCpp.h" // On a besoin de connaître notre classe joueur

ACrouchZone::ACrouchZone()
{
	PrimaryActorTick.bCanEverTick = false;
	ZoneVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneVolume"));
	SetRootComponent(ZoneVolume);
	ZoneVolume->SetCollisionProfileName(TEXT("Trigger"));
}

void ACrouchZone::BeginPlay()
{
	Super::BeginPlay();
	ZoneVolume->OnComponentBeginOverlap.AddDynamic(this, &ACrouchZone::OnVolumeBeginOverlap);
	ZoneVolume->OnComponentEndOverlap.AddDynamic(this, &ACrouchZone::OnVolumeEndOverlap);
}

void ACrouchZone::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFPSPlayer_BaseCpp* PlayerCharacter = Cast<AFPSPlayer_BaseCpp>(OtherActor))
	{
		// On dit au joueur qu'il est maintenant dans une zone où il peut se cacher
		PlayerCharacter->SetIsInCrouchZone(true);
	}
}

void ACrouchZone::OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AFPSPlayer_BaseCpp* PlayerCharacter = Cast<AFPSPlayer_BaseCpp>(OtherActor))
	{
		// On dit au joueur qu'il n'est plus dans une zone où il peut se cacher
		PlayerCharacter->SetIsInCrouchZone(false);
	}
}