#include "VictoryZone.h"
#include "Components/BoxComponent.h"
#include "FPSPlayer_BaseCpp.h"
#include "VictoryWidget.h" // On inclut l'en-tête de notre widget
#include "GameFramework/PlayerController.h"

AVictoryZone::AVictoryZone()
{
    PrimaryActorTick.bCanEverTick = false;
    VictoryVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("VictoryVolume"));
    SetRootComponent(VictoryVolume);
    VictoryVolume->SetCollisionProfileName(TEXT("Trigger"));
}

void AVictoryZone::BeginPlay()
{
    Super::BeginPlay();
    VictoryVolume->OnComponentBeginOverlap.AddDynamic(this, &AVictoryZone::OnZoneOverlap);
}

void AVictoryZone::OnZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // On vérifie que ce n'est pas déjà déclenché et que c'est bien le joueur
    if (!bHasTriggered && Cast<AFPSPlayer_BaseCpp>(OtherActor))
    {
        bHasTriggered = true; // On verrouille pour éviter les déclenchements multiples
        UE_LOG(LogTemp, Warning, TEXT("Player has entered the Victory Zone! Game Over."));

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC && VictoryWidgetClass)
        {
            // On crée le widget
            UVictoryWidget* VictoryWidget = CreateWidget<UVictoryWidget>(PC, VictoryWidgetClass);
            if (VictoryWidget)
            {
                // On l'ajoute à l'écran
                VictoryWidget->AddToViewport();

                // On change le mode d'input pour que le joueur contrôle l'UI et plus le personnage
                FInputModeUIOnly InputModeData;
                InputModeData.SetWidgetToFocus(VictoryWidget->TakeWidget());
                InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

                PC->SetInputMode(InputModeData);
                PC->bShowMouseCursor = true; // On affiche la souris
            }
        }
    }
}