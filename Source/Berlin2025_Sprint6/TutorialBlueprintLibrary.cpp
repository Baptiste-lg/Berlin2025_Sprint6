#include "TutorialBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"   // Pour GetPlayerController
#include "Blueprint/UserWidget.h"     // Pour CreateWidget
// "TutorialWidget.h" est déjà inclus via "TutorialBlueprintLibrary.h"

UTutorialWidget* UTutorialBlueprintLibrary::ShowTutorial(UObject* WorldContextObject, TSubclassOf<UTutorialWidget> TutorialWidgetClass, const TArray<FText>& TutorialMessages)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Error, TEXT("ShowTutorial: WorldContextObject est null."));
        return nullptr;
    }

    if (!TutorialWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ShowTutorial: TutorialWidgetClass n'est pas spécifié."));
        return nullptr;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("ShowTutorial: Impossible d'obtenir le PlayerController."));
        return nullptr;
    }

    if (!PC->IsLocalPlayerController()) // S'assurer que c'est pour le joueur local
    {
         // En mode solo, c'est toujours le cas, mais bonne pratique pour le multijoueur.
        return nullptr;
    }

    UTutorialWidget* TutorialWidgetInstance = CreateWidget<UTutorialWidget>(PC, TutorialWidgetClass);
    if (TutorialWidgetInstance)
    {
        TutorialWidgetInstance->SetTutorialMessages(TutorialMessages);
        TutorialWidgetInstance->AddToViewport();
        TutorialWidgetInstance->InitializeTutorial();
        return TutorialWidgetInstance; // Retourne le widget créé
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ShowTutorial: Impossible de créer l'instance de TutorialWidget."));
    }

    return nullptr;
}

// Si tu ajoutes la fonction HideTutorial :
// void UTutorialBlueprintLibrary::HideTutorial(UTutorialWidget* TutorialWidgetInstance)
// {
//     if (TutorialWidgetInstance && TutorialWidgetInstance->IsVisible())
//     {
//         TutorialWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
//         // ou TutorialWidgetInstance->CloseTutorial(); si tu as une fonction spécifique pour ça
//     }
// }