// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "InfiniteFloorTile.generated.h"

UCLASS()
class FGINFINITERUNNERGAME_API AInfiniteFloorTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInfiniteFloorTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void DestroyAfterDelay(float DelayTime);

private:
	FTimerHandle TimerHandle_Destroy;
	void DestroyActor();

};
