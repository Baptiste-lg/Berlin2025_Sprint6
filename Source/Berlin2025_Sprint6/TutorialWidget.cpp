#include "TutorialWidget.h"
#include "Components/TextBlock.h"
// #include "Components/Button.h" // On n'a plus besoin de Button.h ici

void UTutorialWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Supprimer les liaisons des boutons OnClicked
    // if (PreviousButton) { ... }
    // if (NextButton) { ... }

    // Les textes pour les boutons ne sont plus nécessaires ici
    // DefaultNextButtonText = FText::FromString(TEXT("Suivant"));
    // CloseButtonText = FText::FromString(TEXT("Fermer"));

    SetVisibility(ESlateVisibility::Collapsed);
}

void UTutorialWidget::SetTutorialMessages(const TArray<FText>& Messages)
{
    AllTutorialMessages = Messages;
}

void UTutorialWidget::InitializeTutorial()
{
    if (AllTutorialMessages.Num() > 0)
    {
        CurrentMessageIndex = 0;
        UpdateWidgetContent();
        SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}

// Implémentation des nouvelles fonctions publiques
void UTutorialWidget::ProcessPreviousAction()
{
    if (CurrentMessageIndex > 0)
    {
        CurrentMessageIndex--;
        UpdateWidgetContent();
    }
}

void UTutorialWidget::ProcessNextAction()
{
    // Si on est sur le dernier message, l'action "Suivant" devient "Fermer"
    if (CurrentMessageIndex == AllTutorialMessages.Num() - 1)
    {
        CloseTutorial();
        return;
    }

    // Sinon, on avance au message suivant
    if (CurrentMessageIndex < AllTutorialMessages.Num() - 1)
    {
        CurrentMessageIndex++;
        UpdateWidgetContent();
    }
}

void UTutorialWidget::UpdateWidgetContent()
{
    // Mettre à jour la condition de vérification des pointeurs
    if (!TutorialTextDisplay || !PreviousKeyHintText || !NextKeyHintText)
    {
        UE_LOG(LogTemp, Error, TEXT("TutorialWidget: TutorialTextDisplay ou les KeyHintTexts ne sont pas liés en C++ !"));
        return;
    }

    if (AllTutorialMessages.IsValidIndex(CurrentMessageIndex))
    {
        TutorialTextDisplay->SetText(AllTutorialMessages[CurrentMessageIndex]);

        // Mettre à jour les textes d'indication des touches
        // Note : "TOUCHE_PRÉC" et "TOUCHE_SUIV" sont des placeholders.
        // Tu peux les remplacer par les vraies touches si tu les passes depuis le PlayerController,
        // ou les laisser génériques. L'utilisateur a demandé "touche : previous", donc ce format est proche.
        if (PreviousKeyHintText)
        {
            if (CurrentMessageIndex == 0) // Si c'est le premier message
            {
                PreviousKeyHintText->SetText(FText::FromString(TEXT(""))); // Fermer le tutoriel
            }
            else
            {
                PreviousKeyHintText->SetText(FText::FromString(TEXT("A : Previous")));
            }
            PreviousKeyHintText->SetVisibility(CurrentMessageIndex > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
        }

        if (NextKeyHintText)
        {
            if (CurrentMessageIndex == AllTutorialMessages.Num() - 1) // Si c'est le dernier message
            {
                NextKeyHintText->SetText(FText::FromString(TEXT("E : Close")));
            }
            else
            {
                NextKeyHintText->SetText(FText::FromString(TEXT("E : Next")));
            }
            NextKeyHintText->SetVisibility(ESlateVisibility::HitTestInvisible); // Toujours visible tant que le tuto est actif
        }
    }
    else
    {
        CloseTutorial();
    }
}

void UTutorialWidget::CloseTutorial()
{
    SetVisibility(ESlateVisibility::Collapsed);
}