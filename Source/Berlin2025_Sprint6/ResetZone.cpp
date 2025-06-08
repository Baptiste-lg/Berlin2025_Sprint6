#include "ResetZone.h"
#include "Components/BoxComponent.h"
#include "FPSPlayer_BaseCpp.h"      // On a besoin de connaître la classe du joueur pour le détecter
#include "MyFPSGameMode.h"          // On a besoin de connaître notre GameMode pour appeler le reset
#include "Kismet/GameplayStatics.h"

AResetZone::AResetZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// Création et configuration du volume de détection
	ResetVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ResetVolume"));
	SetRootComponent(ResetVolume);
	ResetVolume->SetCollisionProfileName(TEXT("Trigger"));
}

void AResetZone::BeginPlay()
{
	Super::BeginPlay();

	// On lie notre fonction à l'événement de détection
	ResetVolume->OnComponentBeginOverlap.AddDynamic(this, &AResetZone::OnZoneOverlap);
}

void AResetZone::OnZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// On vérifie si l'acteur qui est entré est bien notre joueur
	if (AFPSPlayer_BaseCpp* PlayerCharacter = Cast<AFPSPlayer_BaseCpp>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has entered a Reset Zone!"));

		// On récupère le GameMode actuel
		if (AMyFPSGameMode* GameMode = GetWorld()->GetAuthGameMode<AMyFPSGameMode>())
		{
			// On appelle la fonction de reset générale que nous avons déjà créée !
			GameMode->ResetPlayerAndEnemies(PlayerCharacter->GetController());
		}
	}
}