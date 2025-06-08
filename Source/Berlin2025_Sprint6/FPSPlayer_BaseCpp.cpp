#include "FPSPlayer_BaseCpp.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TutorialWidget.h"
#include "ResetEffectWidget.h" // <-- NOUVEAU: Include pour le widget d'effet
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MyFPSGameMode.h"
#include "Kismet/GameplayStatics.h"

AFPSPlayer_BaseCpp::AFPSPlayer_BaseCpp()
{
    PrimaryActorTick.bCanEverTick = true;
    CameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimeline"));
}

void AFPSPlayer_BaseCpp::BeginPlay()
{
    Super::BeginPlay();

    if (CameraTimelineCurve)
    {
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("UpdateCameraHeight"));
        CameraTimeline->AddInterpFloat(CameraTimelineCurve, ProgressFunction);
    }

    StartLocation = GetActorLocation();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (TutorialMappingContext)
            {
                Subsystem->AddMappingContext(TutorialMappingContext, 1);
            }
        }

        // CORRECTION : Le code pour créer le widget doit être ici, car il a besoin de 'PC'.
        if (TutorialWidgetClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Creating Tutorial Widget Instance"));
            TutorialWidgetInstance = CreateWidget<UTutorialWidget>(PC, TutorialWidgetClass);
            // 1. On récupère le nom du niveau actuel
            FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

            // 2. On prépare un tableau vide qui contiendra nos messages
            TArray<FText> MessagesForLevel;

            if (CurrentLevelName.Contains("L_DeepCave2"))
            {
                // --- MESSAGES POUR LE NIVEAU 2 ---
                MessagesForLevel = {
                    FText::FromString(TEXT("Welcome to the \nfirst le-\nWhy are they here?")),
                    FText::FromString(TEXT("Player you must \nescape this place.")),
                    FText::FromString(TEXT("They changed the layout \nof the cave.")),
                    FText::FromString(TEXT("The combination must \nstill be the same.")),
                    FText::FromString(TEXT("Use the herbs to \nhide from them.")),
                    FText::FromString(TEXT("SHIFT to crouch.\nR to reset.")),
                };
            }
            else
            {
                // --- MESSAGES PAR DÉFAUT (pour le niveau 1 et tous les autres) ---
                MessagesForLevel = {
                    FText::FromString(TEXT("Welcome to the \ntutorial!")),
                    FText::FromString(TEXT("Use ZQSD to move.")),
                    FText::FromString(TEXT("Left-click to pick up \nor drop crystals.")),
                    FText::FromString(TEXT("Place the correct \ncrystals on their \npedestals.")),
                    FText::FromString(TEXT("Once all crystals are on the \ncorrect pedestals, \nthe door will open."))
                };
            }
                TutorialWidgetInstance->SetTutorialMessages(MessagesForLevel);
                TutorialWidgetInstance->AddToViewport();
                TutorialWidgetInstance->InitializeTutorial();
        }
    }
}

void AFPSPlayer_BaseCpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (TutorialNextAction)
        {
            EnhancedInputComponent->BindAction(TutorialNextAction, ETriggerEvent::Triggered, this, &AFPSPlayer_BaseCpp::HandleTutorialNext);
        }
        if (TutorialPreviousAction)
        {
            EnhancedInputComponent->BindAction(TutorialPreviousAction, ETriggerEvent::Triggered, this, &AFPSPlayer_BaseCpp::HandleTutorialPrevious);
        }
        if (CrouchAction)
        {
            EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFPSPlayer_BaseCpp::HandleCrouch);
        }
        // CORRECTION : La liaison pour l'action de reset doit être ici.
        if (ResetAction)
        {
            EnhancedInputComponent->BindAction(ResetAction, ETriggerEvent::Triggered, this, &AFPSPlayer_BaseCpp::HandleReset);
        }
    }
}

void AFPSPlayer_BaseCpp::ResetToStartLocation()
{
    UE_LOG(LogTemp, Warning, TEXT("Resetting Player to start location."));
    SetActorLocation(StartLocation);

    // NOUVEAU : On déclenche l'effet de "jumpscare" juste après la téléportation
    if (ResetEffectWidgetClass)
    {
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            UResetEffectWidget* EffectWidget = CreateWidget<UResetEffectWidget>(PC, ResetEffectWidgetClass);
            if (EffectWidget)
            {
                EffectWidget->AddToViewport(10); // Priorité 10 pour être par-dessus tout
                EffectWidget->StartEffect();
            }
        }
    }
}

void AFPSPlayer_BaseCpp::HandleReset(const FInputActionValue& Value)
{
    if (AMyFPSGameMode* GameMode = GetWorld()->GetAuthGameMode<AMyFPSGameMode>())
    {
        GameMode->ResetPlayerAndEnemies(GetController());
    }
}

void AFPSPlayer_BaseCpp::HandleCrouch(const FInputActionValue& Value)
{
    if (!bCanToggleCrouch)
    {
        return;
    }
    bIsCustomCrouched = !bIsCustomCrouched;
    if (bIsCustomCrouched)
    {
        CameraTimeline->Play();
    }
    else
    {
        CameraTimeline->Reverse();
    }
    bCanToggleCrouch = false;
    GetWorld()->GetTimerManager().SetTimer(CrouchCooldownTimerHandle, this, &AFPSPlayer_BaseCpp::ResetCrouchCooldown, 1.0f, false);
}

void AFPSPlayer_BaseCpp::UpdateCameraHeight(float Alpha)
{
    if (PlayerCameraComponent)
    {
        float NewZ = FMath::Lerp(StandingCameraHeight, CrouchingCameraHeight, Alpha);
        FVector CurrentLocation = PlayerCameraComponent->GetRelativeLocation();
        PlayerCameraComponent->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZ));
    }
}

void AFPSPlayer_BaseCpp::ResetCrouchCooldown()
{
    bCanToggleCrouch = true;
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

void AFPSPlayer_BaseCpp::SetIsInCrouchZone(bool bInZone)
{
    bIsInCrouchZone = bInZone;
}

bool AFPSPlayer_BaseCpp::IsCurrentlyHidden() const
{
    return bIsInCrouchZone && bIsCustomCrouched;
}