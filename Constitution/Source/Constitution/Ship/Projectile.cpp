// Fill out your copyright notice in the Description page of Project Settings.

#include "Constitution.h"
#include "Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (GetActorLocation().Z < -100.0f)
    {
        Destroy();
    }
}

