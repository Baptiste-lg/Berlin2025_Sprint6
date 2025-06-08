#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "FPSPlayer_BaseCpp.generated.h"

class UInputMappingContext;
class UInputAction;
class UTutorialWidget;
class UCameraComponent;
class UCurveFloat;
class UResetEffectWidget; // <-- NOUVEAU: Déclaration anticipée pour notre widget d'effet
struct FInputActionValue;

UCLASS()
class BERLIN2025_SPRINT6_API AFPSPlayer_BaseCpp : public ACharacter // Remplace par le nom de ton projet
{
    GENERATED_BODY()

public:
    AFPSPlayer_BaseCpp();
    void SetIsInCrouchZone(bool bInZone);
    bool IsCurrentlyHidden() const;
    void ResetToStartLocation();

protected:
    // Fonctions standards d'Unreal
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ~~~ PROPRIÉTÉS DU TUTORIEL ~~~
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | Input")
    TObjectPtr<UInputMappingContext> TutorialMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | Input")
    TObjectPtr<UInputAction> TutorialNextAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | Input")
    TObjectPtr<UInputAction> TutorialPreviousAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tutorial System | UI")
    TSubclassOf<UTutorialWidget> TutorialWidgetClass;

    // --- NOUVEAU ---
    // ~~~ PROPRIÉTÉ DE L'EFFET DE RESET ~~~
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UResetEffectWidget> ResetEffectWidgetClass;
    // --- FIN NOUVEAU ---

    // ~~~ PROPRIÉTÉS DES ACTIONS ~~~
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Actions")
    TObjectPtr<UInputAction> CrouchAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Actions")
    TObjectPtr<UInputAction> ResetAction;

    // ~~~ SYSTÈME DE CROUCH PERSONNALISÉ ~~~
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Crouch")
    TObjectPtr<UTimelineComponent> CameraTimeline;

    UPROPERTY(EditDefaultsOnly, Category = "Custom Crouch")
    TObjectPtr<UCurveFloat> CameraTimelineCurve;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Crouch")
    TObjectPtr<UCameraComponent> PlayerCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Crouch")
    bool bIsCustomCrouched = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Crouch")
    bool bIsInCrouchZone = false;

    float StandingCameraHeight = 64.f;
    float CrouchingCameraHeight = 35.f;

private:
    UPROPERTY()
    TObjectPtr<UTutorialWidget> TutorialWidgetInstance;

    bool bCanToggleCrouch = true;
    FTimerHandle CrouchCooldownTimerHandle;
    void ResetCrouchCooldown();

    void HandleTutorialNext(const FInputActionValue& Value);
    void HandleTutorialPrevious(const FInputActionValue& Value);
    void HandleCrouch(const FInputActionValue& Value);
    void HandleReset(const FInputActionValue& Value);

    UFUNCTION()
    void UpdateCameraHeight(float Alpha);

    FVector StartLocation;
};