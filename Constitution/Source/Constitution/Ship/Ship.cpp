// Fill out your copyright notice in the Description page of Project Settings.

#include "Constitution.h"
#include "Ship.h"

AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShip::BeginPlay()
{
	Super::BeginPlay();

    // allow inputs to ship
    EnableInput(GetWorld()->GetFirstPlayerController());

    // find cannons
    TArray<AActor*> tempChildActors;
    GetAllChildActors(tempChildActors, true);

    for (auto& child : tempChildActors)
    {
        if (child->IsA(AArtillery::StaticClass()))
        {
            Artillery artillery = Artillery();
            artillery.artillery = Cast<AArtillery>(child);
            artillery.position = GetTransform().InverseTransformPosition(artillery.artillery->GetActorLocation());

            TArray<Artillery>* sideArtillery = (artillery.position.X < 0) ? &m_rightArtillery : &m_leftArtillery;
            sideArtillery->Add(artillery);
        }
    }
}

void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (AutoFire)
    {
        FireCannons();
    }
}

void AShip::FireCannons()
{
    FireCannons(&m_rightArtillery);
}

void AShip::FireCannons(TArray<Artillery>* artillery)
{
    // get the position of the first fireable cannon
    float rollStartPos = 0;
    for (auto& a : (*artillery))
    {
        if (a.artillery->CanFire())
        {
            if (rollStartPos < a.position.Y)
            {
                rollStartPos = a.position.Y;
            }
        }
    }

    // fire all fireable cannons, delayed by distance relative to the first fireable cannon
    for (auto& a : (*artillery))
    {
        if (a.artillery->CanFire())
        {
            a.artillery->Fire((rollStartPos - a.position.Y) / (100 * RollingFireSpeed));
        }
    }
}

void AShip::ToggleAutoFire()
{
    AutoFire = !AutoFire;
}

