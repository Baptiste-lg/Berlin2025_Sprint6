#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionZone.generated.h"

// Déclaration anticipée pour le composant de collision
class UBoxComponent;

UCLASS()
class BERLIN2025_SPRINT6_API ALevelTransitionZone : public AActor // Remplace YOURPROJECT_API
{
	GENERATED_BODY()

public:
	// Constructeur
	ALevelTransitionZone();

protected:
	// Appelé au démarrage du jeu
	virtual void BeginPlay() override;

	// --- Composants ---

	// La zone de collision qui détectera le joueur
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TransitionZone;

	// --- Propriétés Configurables ---

	// Variable pour définir le nom du niveau à charger.
	// "EditAnywhere" permet de la modifier directement sur une instance placée dans le niveau.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition")
	FName NextLevelName;

	// --- Logique de Détection ---

	// Fonction qui sera appelée lorsque quelque chose entre dans la zone.
	// Le UFUNCTION() est nécessaire pour la lier à l'événement de collision.
	UFUNCTION()
	void OnZoneBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};