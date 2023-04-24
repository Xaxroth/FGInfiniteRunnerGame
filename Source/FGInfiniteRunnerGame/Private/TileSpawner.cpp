// Fill out your copyright notice in the Description page of Project Settings.

#include "TileSpawner.h"
#include "Engine/World.h"

// Sets default values
ATileSpawner::ATileSpawner()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ATileSpawner::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ATileSpawner::OnOverlap);
}

// Called every frame
void ATileSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ATileSpawner::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor != this && OtherActor != GetOwner())
    {
        SpawnFloorTile();
    }
}

void ATileSpawner::SpawnFloorTile()
{
    int32 RandomIndex = FMath::RandRange(0, ActorsToSpawn.Num() - 1);

    FVector Location = GetActorLocation();
    Location.X += 6000.0f;

    FRotator Rotation = GetActorRotation();

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorsToSpawn[RandomIndex], Location, Rotation, SpawnParameters);
}

