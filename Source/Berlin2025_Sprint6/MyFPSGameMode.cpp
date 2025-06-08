#include "MyFPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FPSPlayer_BaseCpp.h" // A besoin de connaître le joueur
#include "Enemy_AI.h"          // et l'ennemi

void AMyFPSGameMode::ResetPlayerAndEnemies(AController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("--- GAME MODE: Resetting level state. ---"));

	// 1. Réinitialiser le joueur
	if (AFPSPlayer_BaseCpp* Player = Cast<AFPSPlayer_BaseCpp>(PlayerController->GetPawn()))
	{
		Player->ResetToStartLocation();
	}

	// 2. Réinitialiser tous les ennemis
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy_AI::StaticClass(), FoundEnemies);

	for (AActor* Actor : FoundEnemies)
	{
		if (AEnemy_AI* Enemy = Cast<AEnemy_AI>(Actor))
		{
			Enemy->ResetToStartLocation();
		}
	}
}