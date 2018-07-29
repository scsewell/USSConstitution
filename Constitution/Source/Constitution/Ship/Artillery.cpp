// Fill out your copyright notice in the Description page of Project Settings.

#include "Constitution.h"
#include "Artillery.h"

AArtillery::AArtillery()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArtillery::BeginPlay()
{
	Super::BeginPlay();
    
    // get particle components
    TArray<UParticleSystemComponent*> particleComponents;
    GetComponents(particleComponents);
    if (particleComponents.Num() > 0)
    {
        m_fireParticles = particleComponents[0];
    }

    // get audio sources
    TArray<UAudioComponent*> audioComponents;
    GetComponents(audioComponents);
    for (auto& audio : audioComponents)
    {
        if (audio->GetName().Contains("Fire"))
        {
            m_fireSound = audio;
        }
    }
}

void AArtillery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (m_firing)
    {
        float timeSinceFire = GetWorld()->GetTimeSeconds() - m_fireTime;

        if (timeSinceFire >= 0 && !m_firedShot)
        {
            if (m_fireParticles != nullptr)
            {
                m_fireParticles->ActivateSystem();
            }
            if (m_fireSound != nullptr)
            {
                m_fireSound->Play();
            }
            m_firedShot = true;
        }

        float timeSinceRunout = timeSinceFire - m_reloadDuration;
        if (timeSinceRunout < 0)
        {
            m_recoil = FMath::Clamp(0.5f * timeSinceFire / RecoilTime, 0.0f, 0.5f);
        }
        else
        {
            if (timeSinceRunout - m_runoutDuration > 0)
            {
                m_firing = false;
            }
            m_recoil = FMath::Clamp(0.5f * timeSinceRunout / m_runoutDuration, 0.0f, 0.5f) + 0.5f;
        }

        SetRecoil(m_recoil);
    }
}

void AArtillery::Fire(float delay)
{
    if (CanFire())
    {
        m_firing = true;
        m_fireTime = GetWorld()->GetTimeSeconds() + delay + FMath::RandRange(0.0f, FireTimeVariance);
        m_reloadDuration = ReloadTime + FMath::RandRange(0.0f, ReloadVariance);
        m_runoutDuration = RunoutTime + FMath::RandRange(0.0f, RunoutVariance);
        m_firedShot = false;
    }
}

void AArtillery::SetRecoil_Implementation(float recoil)
{
}

bool AArtillery::CanFire()
{
    return !m_firing;
}