#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

class UTextBlock;
// class UButton; // On n'a plus besoin de UButton ici

UCLASS()
class BERLIN2025_SPRINT6_API UTutorialWidget : public UUserWidget // Remplace YOURPROJECT_API
{
    GENERATED_BODY()

public:
    void SetTutorialMessages(const TArray<FText>& Messages);
    void InitializeTutorial();

    // Ajoute UFUNCTION(BlueprintCallable) ici !
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ProcessPreviousAction();

    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ProcessNextAction();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TutorialTextDisplay; // Reste inchangé

    // Supprimer les UPROPERTY pour les UButton
    // UPROPERTY(meta = (BindWidget))
    // UButton* PreviousButton;
    // UPROPERTY(meta = (BindWidget))
    // UButton* NextButton;
    // UPROPERTY(meta = (BindWidget))
    // UTextBlock* NextButtonText; // Était lié au NextButton

    // Nouveaux UPROPERTY pour les indications de touches
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PreviousKeyHintText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* NextKeyHintText;

private:
    // Les fonctions OnClicked ne sont plus nécessaires
    // UFUNCTION()
    // void OnPreviousClicked();
    // UFUNCTION()
    // void OnNextClicked();

    void UpdateWidgetContent();
    void CloseTutorial();

    TArray<FText> AllTutorialMessages;
    int32 CurrentMessageIndex = 0;

    // DefaultNextButtonText et CloseButtonText ne sont plus utiles pour les boutons
    // FText DefaultNextButtonText;
    // FText CloseButtonText;
};