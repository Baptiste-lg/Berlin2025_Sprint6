#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h" // Nécessaire pour FAIStimulus
#include "Enemy_AI.generated.h"

class UAIPerceptionComponent;
class USoundBase; // <-- Déclaration anticipée pour notre son


UCLASS()
class BERLIN2025_SPRINT6_API AEnemy_AI : public ACharacter // Remplace par le nom de ton projet
{
	GENERATED_BODY()

public:
	AEnemy_AI();
	void ResetToStartLocation();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ~~~ PROPRIÉTÉS DE MOUVEMENT (Réglables dans le Blueprint) ~~~
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
	float NormalSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
	float ChaseSpeed = 1000.0f;

	// --- Composants ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	// --- Fonctions de gestion d'événements ---
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Sound")
	TObjectPtr<USoundBase> DetectionSound;

	UFUNCTION()
	void OnPlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector InitialLocation;

	UPROPERTY()
	TObjectPtr<AActor> TargetPlayer;

	bool bIsChasing = false;

	// Booléen pour savoir si le stimulus de perception est actuellement actif
	bool bIsPlayerPerceived = false;
};