#include "FPSPlayer_BaseCpp.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TutorialWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

AFPSPlayer_BaseCpp::AFPSPlayer_BaseCpp()
{
    // Le constructeur est maintenant vide car les composants (caméra, mesh...)
    // sont gérés par ton Blueprint.
 	PrimaryActorTick.bCanEverTick = true;
}

void AFPSPlayer_BaseCpp::BeginPlay()
{
	// Important : On appelle la fonction du parent pour s'assurer que tout fonctionne correctement
	Super::BeginPlay();

    // --- Logique d'initialisation du tutoriel ---
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        // Ajout du contexte d'input pour le tutoriel
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (TutorialMappingContext)
            {
                Subsystem->AddMappingContext(TutorialMappingContext, 1); // Priorité 1
            }
        }

        // Création et affichage du widget de tutoriel
        if (TutorialWidgetClass)
        {
            TutorialWidgetInstance = CreateWidget<UTutorialWidget>(PC, TutorialWidgetClass);
            if (TutorialWidgetInstance)
            {
                // Les messages du tutoriel sont définis ici.
                TArray<FText> Messages = {
                    FText::FromString(TEXT("Use ZQSD to move.")),
                    FText::FromString(TEXT("Left-click to pick up \nor drop crystals.")),
                    FText::FromString(TEXT("Place the correct \ncrystals on their \npedestals.")),
                    FText::FromString(TEXT("Once all crystals are on the \ncorrect pedestals, \nthe door will open."))
                };
                TutorialWidgetInstance->SetTutorialMessages(Messages);
                TutorialWidgetInstance->AddToViewport();
                TutorialWidgetInstance->InitializeTutorial();
            }
        }
    }
}

void AFPSPlayer_BaseCpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // --- Liaison des entrées clavier du tutoriel ---
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Lie les Input Actions aux fonctions de gestion C++
        if (TutorialNextAction)
        {
            EnhancedInputComponent->BindAction(TutorialNextAction, ETriggerEvent::Triggered, this, &AFPSPlayer_BaseCpp::HandleTutorialNext);
        }
        if (TutorialPreviousAction)
        {
            EnhancedInputComponent->BindAction(TutorialPreviousAction, ETriggerEvent::Triggered, this, &AFPSPlayer_BaseCpp::HandleTutorialPrevious);
        }
    }
}

void AFPSPlayer_BaseCpp::HandleTutorialNext(const FInputActionValue& Value)
{
    if (TutorialWidgetInstance && TutorialWidgetInstance->IsVisible())
    {
        TutorialWidgetInstance->ProcessNextAction();
    }
}

void AFPSPlayer_BaseCpp::HandleTutorialPrevious(const FInputActionValue& Value)
{
    if (TutorialWidgetInstance && TutorialWidgetInstance->IsVisible())
    {
        TutorialWidgetInstance->ProcessPreviousAction();
    }
}