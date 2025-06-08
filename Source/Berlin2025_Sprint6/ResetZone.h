#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResetZone.generated.h"

class UBoxComponent;

UCLASS()
class BERLIN2025_SPRINT6_API AResetZone : public AActor
{
	GENERATED_BODY()

public:
	AResetZone();

protected:
	virtual void BeginPlay() override;

	// La zone de détection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> ResetVolume;

	// La fonction qui sera appelée lorsque le joueur entre dans la zone
	UFUNCTION()
	void OnZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};