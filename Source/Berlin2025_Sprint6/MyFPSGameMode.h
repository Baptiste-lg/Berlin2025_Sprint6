#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyFPSGameMode.generated.h"

UCLASS()
class BERLIN2025_SPRINT6_API AMyFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Fonction publique qui peut être appelée de n'importe où pour tout réinitialiser
	void ResetPlayerAndEnemies(AController* PlayerController);
};