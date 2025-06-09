#include "VictoryWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" // Nécessaire pour quitter le jeu

void UVictoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// On lie nos fonctions C++ aux événements de clic des boutons
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UVictoryWidget::OnRestartClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UVictoryWidget::OnQuitClicked);
	}
}

void UVictoryWidget::OnRestartClicked()
{
	// On vérifie qu'un nom de niveau a bien été spécifié
	if (!TutorialLevelName.IsNone())
	{
		// On charge le niveau du tutoriel (le premier niveau)
		UGameplayStatics::OpenLevel(this, TutorialLevelName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VictoryWidget: TutorialLevelName n'est pas défini !"));
	}
}

void UVictoryWidget::OnQuitClicked()
{
	// On récupère le Player Controller pour lui donner l'ordre de quitter
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Fonction standard pour fermer le jeu
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);
}