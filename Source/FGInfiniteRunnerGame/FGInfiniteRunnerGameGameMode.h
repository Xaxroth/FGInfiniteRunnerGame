// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FGInfiniteRunnerGameGameMode.generated.h"

UCLASS(minimalapi)
class AFGInfiniteRunnerGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFGInfiniteRunnerGameGameMode();

	int32 PlayerHealth = 3;
	int32 PlayerScore = 3;

	bool InvincibilityFrames;
	float InvincibilityDuration = 3.f;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> InitialTile;

public:
	void LoseHealth(float value);

	void GameOver();

	void IncreaseScore(float value);

public:
	UFUNCTION(BlueprintCallable)
		void GiveInvincibilityFrames(float Duration);

	void ReloadLevel();

private:
	FTimerHandle TimerHandle_Destroy;
	void MakeVulnerable();

};



