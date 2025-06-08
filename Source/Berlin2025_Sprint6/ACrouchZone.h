#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACrouchZone.generated.h"

class UBoxComponent;

UCLASS()
class BERLIN2025_SPRINT6_API ACrouchZone : public AActor
{
	GENERATED_BODY()

public:
	ACrouchZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> ZoneVolume;

	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};