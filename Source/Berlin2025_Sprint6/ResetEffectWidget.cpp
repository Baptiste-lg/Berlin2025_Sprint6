#include "ResetEffectWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h" // Nécessaire pour la fonction FMath::RandRange

void UResetEffectWidget::StartEffect()
{
    // On s'assure que la liste d'images principale n'est pas vide
    if (JumpscareImages.Num() == 0)
    {
        StopEffect();
        return;
    }

    // --- Logique de Randomisation ---
    // 1. On vide la liste aléatoire d'une exécution précédente
    RandomizedImageList.Empty();

    // 2. On choisit combien d'images on va montrer cette fois-ci (entre Min et Max définis dans le Blueprint)
    int32 ImageCountToShow = FMath::RandRange(NumberOfImagesToShow.X, NumberOfImagesToShow.Y);

    // 3. On crée une copie de la liste d'images disponibles pour ne pas modifier l'original
    TArray<TObjectPtr<UTexture2D>> ImagePool = JumpscareImages;

    // 4. On pioche au hasard dans le "pool" pour remplir notre liste pour cet effet
    for (int32 i = 0; i < ImageCountToShow; ++i)
    {
        // Si le pool est vide (parce qu'on a demandé plus d'images qu'il n'y en a de disponibles), on arrête
        if (ImagePool.Num() == 0)
        {
            break;
        }

        // On choisit un index au hasard dans le pool restant
        int32 RandomIndex = FMath::RandRange(0, ImagePool.Num() - 1);

        // On ajoute l'image choisie à notre liste temporaire pour l'effet
        RandomizedImageList.Add(ImagePool[RandomIndex]);

        // On retire l'image du pool pour éviter de la piocher une deuxième fois
        ImagePool.RemoveAt(RandomIndex);
    }

    // Si, pour une raison quelconque, notre liste finale est vide, on arrête l'effet
    if (RandomizedImageList.Num() == 0)
    {
        StopEffect();
        return;
    }

    // --- Déclenchement de l'Effet ---
    CurrentImageIndex = 0;

    if (JumpscareSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), JumpscareSound);
    }

    // On affiche la première image de notre nouvelle liste aléatoire
    ChangeImage();

    // On lance un minuteur qui appellera ChangeImage() en boucle
    GetWorld()->GetTimerManager().SetTimer(
        ImageChangeTimerHandle,
        this,
        &UResetEffectWidget::ChangeImage,
        ImageDisplayTime,
        true
    );

    // On calcule la durée totale en fonction du nombre d'images réellement choisies
    float TotalDuration = RandomizedImageList.Num() * ImageDisplayTime;

    // On lance un autre minuteur pour arrêter l'effet complet
    GetWorld()->GetTimerManager().SetTimer(
        StopEffectTimerHandle,
        this,
        &UResetEffectWidget::StopEffect,
        TotalDuration,
        false
    );
}

void UResetEffectWidget::ChangeImage()
{
    // On itère simplement à travers notre liste aléatoire temporaire
    if (DisplayImage && RandomizedImageList.IsValidIndex(CurrentImageIndex))
    {
        DisplayImage->SetBrushFromTexture(RandomizedImageList[CurrentImageIndex]);
        CurrentImageIndex++;
    }
    else
    {
        // Si on a montré toutes les images de la séquence, on arrête de changer l'image.
        // Le minuteur StopEffect s'occupera de fermer le widget au bon moment.
        GetWorld()->GetTimerManager().ClearTimer(ImageChangeTimerHandle);
    }
}

void UResetEffectWidget::StopEffect()
{
    // On nettoie les minuteurs et on retire le widget de l'écran
    GetWorld()->GetTimerManager().ClearTimer(ImageChangeTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(StopEffectTimerHandle);
    RemoveFromParent();
}