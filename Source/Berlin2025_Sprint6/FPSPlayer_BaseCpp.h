#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlayer_BaseCpp.generated.h"

// Déclarations anticipées pour ne pas surcharger les en-têtes
class UInputMappingContext;
class UInputAction;
class UTutorialWidget;
struct FInputActionValue;

UCLASS()
class BERLIN2025_SPRINT6_API AFPSPlayer_BaseCpp : public ACharacter // Remplace YOURPROJECT_API
{
    GENERATED_BODY()

public:
    // Constructeur
    AFPSPlayer_BaseCpp();

protected:
    // Appelé au démarrage du jeu pour initialiser notre système de tutoriel
    virtual void BeginPlay() override;

    // Appelé pour lier les entrées clavier aux fonctions
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ~~~ PROPRIÉTÉS DU SYSTÈME DE TUTORIEL ~~~
    // Ces variables seront les seules exposées par ce code.
    // Tu pourras les configurer dans les "Class Defaults" de ton Blueprint ABP_FPSPlayer.

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | Input")
    TObjectPtr<UInputMappingContext> TutorialMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | Input")
    TObjectPtr<UInputAction> TutorialNextAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | Input")
    TObjectPtr<UInputAction> TutorialPreviousAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | UI")
    TSubclassOf<UTutorialWidget> TutorialWidgetClass;

private:
    // Instance du widget de tutoriel, gérée en interne par ce code C++
    UPROPERTY()
    TObjectPtr<UTutorialWidget> TutorialWidgetInstance;

    // Fonctions de gestion pour les entrées du tutoriel
    void HandleTutorialNext(const FInputActionValue& Value);
    void HandleTutorialPrevious(const FInputActionValue& Value);
};