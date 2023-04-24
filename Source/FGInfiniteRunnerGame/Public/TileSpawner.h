// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TileSpawner.generated.h"

UCLASS()
class FGINFINITERUNNERGAME_API ATileSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATileSpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<TSubclassOf<AActor>> ActorsToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnFloorTile();

public:
	UFUNCTION()
		void OnOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ActorToSpawn;
};
