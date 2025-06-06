#include "LevelTransitionZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h" // Nécessaire pour la fonction OpenLevel
#include "FPSPlayer_BaseCpp.h"     // Nécessaire pour vérifier si l'acteur qui entre est bien notre joueur

ALevelTransitionZone::ALevelTransitionZone()
{
    // Le tick n'est pas nécessaire pour cet acteur
    PrimaryActorTick.bCanEverTick = false;

    // Création de la zone de collision (BoxComponent)
    TransitionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionZone"));
    SetRootComponent(TransitionZone); // On le définit comme composant racine

    // Configuration de la collision
    // On utilise le profil de collision "Trigger" qui est conçu pour des zones de détection.
    // Il détecte les chevauchements ("Overlap") mais n'a pas de collision physique.
    TransitionZone->SetCollisionProfileName(TEXT("Trigger"));
    TransitionZone->SetBoxExtent(FVector(100.f, 100.f, 100.f)); // Taille par défaut, tu pourras la changer dans l'éditeur
}

void ALevelTransitionZone::BeginPlay()
{
    Super::BeginPlay();

    // On lie notre fonction C++ OnZoneBeginOverlap à l'événement OnComponentBeginOverlap du BoxComponent.
    // C'est cette ligne qui fait le lien entre la détection et notre code.
    if (TransitionZone)
    {
        TransitionZone->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionZone::OnZoneBeginOverlap);
    }
}

void ALevelTransitionZone::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Vérifie si un acteur est entré dans la zone et si ce n'est pas lui-même
    if (OtherActor && OtherActor != this)
    {
        // On vérifie que l'acteur qui est entré est bien notre joueur (AFPSPlayer_BaseCpp).
        // C'est une sécurité pour éviter que, par exemple, un objet qui tombe ne déclenche le changement de niveau.
        AFPSPlayer_BaseCpp* PlayerCharacter = Cast<AFPSPlayer_BaseCpp>(OtherActor);
        if (PlayerCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player has entered the transition zone."));

            // On vérifie que le nom du niveau suivant a bien été spécifié dans l'éditeur.
            if (!NextLevelName.IsNone())
            {
                UE_LOG(LogTemp, Warning, TEXT("Loading level: %s"), *NextLevelName.ToString());

                // On utilise la fonction standard d'Unreal pour charger le niveau.
                UGameplayStatics::OpenLevel(this, NextLevelName);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("NextLevelName is not set on the LevelTransitionZone!"));
            }
        }
    }
}