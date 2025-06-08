#include "Enemy_AI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "FPSPlayer_BaseCpp.h"
#include "AIController.h"
#include "MyFPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundBase.h" // Include pour le son

AEnemy_AI::AEnemy_AI()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
    }

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 1500.f;
        SightConfig->LoseSightRadius = 2000.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

        PerceptionComponent->ConfigureSense(*SightConfig);
        PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
    }
}

void AEnemy_AI::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = GetActorLocation();

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }

    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_AI::OnPerceptionUpdated);
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_AI::OnPlayerCollision);

    TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemy_AI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TargetPlayer) return;

    AFPSPlayer_BaseCpp* Player = Cast<AFPSPlayer_BaseCpp>(TargetPlayer);
    if (!Player) return;

    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        // 1. Logique de rotation (inchangée)
        FVector StartLocation = GetActorLocation();
        FVector TargetLocation = TargetPlayer->GetActorLocation();
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
        FRotator CurrentRotation = AIController->GetControlRotation();
        CurrentRotation.Yaw = LookAtRotation.Yaw;
        AIController->SetControlRotation(CurrentRotation);

        // 2. Logique de décision (mise à jour)
        bool bShouldBeChasing = bIsPlayerPerceived && !Player->IsCurrentlyHidden();

        // On vérifie s'il y a un changement d'état de "non-poursuite" à "poursuite"
        if (bShouldBeChasing && !bIsChasing)
        {
            // C'est le moment exact de la détection ! On joue le son.
            if (DetectionSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, DetectionSound, GetActorLocation());
            }
        }

        // On met à jour notre variable de "mémoire" pour la prochaine frame.
        bIsChasing = bShouldBeChasing;

        // 3. Logique de mouvement basée sur l'état
        if (bIsChasing)
        {
            GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
            AIController->MoveToActor(Player, 1.f);
        }
        else
        {
            GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
            AIController->StopMovement();
        }
    }
}

void AEnemy_AI::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor != TargetPlayer) return;

    // Met simplement à jour si oui ou non le joueur est perçu
    bIsPlayerPerceived = Stimulus.WasSuccessfullySensed();
}

void AEnemy_AI::OnPlayerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AFPSPlayer_BaseCpp* Player = Cast<AFPSPlayer_BaseCpp>(OtherActor))
    {
        if (AMyFPSGameMode* GameMode = GetWorld()->GetAuthGameMode<AMyFPSGameMode>())
        {
            GameMode->ResetPlayerAndEnemies(Player->GetController());
        }
    }
}

void AEnemy_AI::ResetToStartLocation()
{
    UE_LOG(LogTemp, Warning, TEXT("Resetting Enemy to start location."));

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }

    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        AIController->StopMovement();
    }

    // On réinitialise aussi nos états internes
    bIsChasing = false;
    bIsPlayerPerceived = false; // Important pour que l'IA ne se remette pas à courir instantanément

    SetActorLocation(InitialLocation);
}