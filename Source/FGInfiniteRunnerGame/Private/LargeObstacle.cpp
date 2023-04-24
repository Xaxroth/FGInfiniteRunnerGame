// Fill out your copyright notice in the Description page of Project Settings.


#include "LargeObstacle.h"

// Sets default values
ALargeObstacle::ALargeObstacle()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ALargeObstacle::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ALargeObstacle::OnOverlap);
}

// Called every frame
void ALargeObstacle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALargeObstacle::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor != this && OtherActor != GetOwner())
    {
        AFGInfiniteRunnerGameGameMode* GameMode = Cast<AFGInfiniteRunnerGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode != nullptr)
        {
            GameMode->LoseHealth(3);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("lost life"));
        }
    }
}


