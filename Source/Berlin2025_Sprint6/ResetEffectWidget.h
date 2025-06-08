#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResetEffectWidget.generated.h"

class UImage;
class USoundBase;

UCLASS()
class BERLIN2025_SPRINT6_API UResetEffectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Fonction principale pour démarrer l'effet
	void StartEffect();

protected:
	// Le composant Image de notre UMG qui affichera les différentes images
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DisplayImage;

	// --- PROPRIÉTÉS CONFIGURABLES DANS LE BLUEPRINT ---

	// Le tableau d'images à faire défiler
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TArray<TObjectPtr<UTexture2D>> JumpscareImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	TObjectPtr<USoundBase> JumpscareSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	float ImageDisplayTime = 0.08f;

	// La durée totale de l'effet avant qu'il ne disparaisse
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties")
	float TotalEffectDuration = 0.48f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Properties", meta = (ClampMin = "1"))
	FIntPoint NumberOfImagesToShow = FIntPoint(4, 6);

private:
	void ChangeImage();
	void StopEffect();

	FTimerHandle ImageChangeTimerHandle;
	FTimerHandle StopEffectTimerHandle;

	int32 CurrentImageIndex = 0;

	// Liste temporaire qui contiendra les images choisies au hasard pour un seul effet.
	TArray<TObjectPtr<UTexture2D>> RandomizedImageList;
};