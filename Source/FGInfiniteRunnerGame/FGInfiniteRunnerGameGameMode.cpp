// Copyright Epic Games, Inc. All Rights Reserved.

#include "FGInfiniteRunnerGameGameMode.h"
#include "FGInfiniteRunnerGameCharacter.h"
#include "TileSpawner.h"
#include "UObject/ConstructorHelpers.h"

AFGInfiniteRunnerGameGameMode::AFGInfiniteRunnerGameGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AFGInfiniteRunnerGameGameMode::IncreaseScore(float value)
{
    PlayerScore += value;
}

void AFGInfiniteRunnerGameGameMode::LoseHealth(float value)
{
    if (InvincibilityFrames == false)
    {
        PlayerHealth -= value;

        GiveInvincibilityFrames(InvincibilityDuration);

        if (PlayerHealth > 0)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("lost life"));
        }
        else
        {
            // PLAYER DIES
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("PLAYER DIES"));
            ReloadLevel();
        }
    }
    else
    {
        return;
    }
}

void AFGInfiniteRunnerGameGameMode::GiveInvincibilityFrames(float Duration)
{
    InvincibilityFrames = true;
    GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &AFGInfiniteRunnerGameGameMode::MakeVulnerable, Duration, false);
}

void AFGInfiniteRunnerGameGameMode::MakeVulnerable()
{
    InvincibilityFrames = false;
}

void AFGInfiniteRunnerGameGameMode::ReloadLevel()
{
    // Get the current level's name
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

    // Open the current level
    UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName), true);
}
