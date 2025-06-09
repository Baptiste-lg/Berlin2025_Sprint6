#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryWidget.generated.h"

class UButton;

UCLASS()
class BERLIN2025_SPRINT6_API UVictoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Est appelé lorsque le widget est construit. C'est ici qu'on lie nos boutons.
	virtual void NativeConstruct() override;

	// --- PROPRIÉTÉ CONFIGURABLE ---
	// On pourra définir le nom du niveau du tutoriel directement dans l'éditeur.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Victory Screen")
	FName TutorialLevelName = "L_DeepCave1"; // Mets ici le nom de ton premier niveau par défaut

	// --- LIAISONS WIDGET (BindWidget) ---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

private:
	// --- Fonctions appelées par les clics des boutons ---
	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnQuitClicked();
};