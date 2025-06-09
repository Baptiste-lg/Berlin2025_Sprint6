#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VictoryZone.generated.h"

class UBoxComponent;
class UVictoryWidget; // Déclaration anticipée

UCLASS()
class BERLIN2025_SPRINT6_API AVictoryZone : public AActor
{
	GENERATED_BODY()

public:
	AVictoryZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> VictoryVolume;

	// Permet de choisir quel Widget de Victoire afficher
	UPROPERTY(EditDefaultsOnly, Category = "Victory Zone")
	TSubclassOf<UVictoryWidget> VictoryWidgetClass;

	UFUNCTION()
	void OnZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// Pour s'assurer que l'événement ne se déclenche qu'une seule fois
	bool bHasTriggered = false;
};