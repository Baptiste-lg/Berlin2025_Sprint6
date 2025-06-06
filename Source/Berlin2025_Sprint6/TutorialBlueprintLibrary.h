#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TutorialWidget.h" // Assure-toi d'inclure le .h de ton widget
#include "TutorialBlueprintLibrary.generated.h"

UCLASS()
class BERLIN2025_SPRINT6_API UTutorialBlueprintLibrary : public UBlueprintFunctionLibrary // Remplace YOURPROJECT_API
{
	GENERATED_BODY()

public:
	/**
	 * Crée et affiche le widget de tutoriel.
	 * @param WorldContextObject Fournit le contexte du monde (par exemple, 'self' depuis un Actor ou PlayerController).
	 * @param TutorialWidgetClass La classe Blueprint de votre UTutorialWidget (par exemple, WBP_Tutorial).
	 * @param TutorialMessages La liste des messages à afficher dans le tutoriel.
	 * @return Une référence au widget de tutoriel créé, ou nullptr en cas d'échec.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tutorial System", meta = (WorldContext = "WorldContextObject"))
	static UTutorialWidget* ShowTutorial(UObject* WorldContextObject, TSubclassOf<UTutorialWidget> TutorialWidgetClass, const TArray<FText>& TutorialMessages);

	// Tu pourrais ajouter d'autres fonctions utilitaires ici, par exemple :
	// UFUNCTION(BlueprintCallable, Category = "Tutorial System")
	// static void HideTutorial(UTutorialWidget* TutorialWidgetInstance);
};