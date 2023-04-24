// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteFloorTile.h"

// Sets default values
AInfiniteFloorTile::AInfiniteFloorTile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInfiniteFloorTile::BeginPlay()
{
	Super::BeginPlay();

	DestroyAfterDelay(15.f);
}

void AInfiniteFloorTile::DestroyAfterDelay(float DelayTime)
{
	GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &AInfiniteFloorTile::DestroyActor, DelayTime, false);
}

void AInfiniteFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInfiniteFloorTile::DestroyActor()
{
	Destroy();
}

