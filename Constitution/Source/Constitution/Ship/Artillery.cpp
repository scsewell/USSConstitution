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
    
    // get mesh component
    m_mesh = FindComponentByClass<USkinnedMeshComponent>();

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
        FString name = audio->GetName();
        if (name.Contains("Fire"))
        {
            m_fireSounds.Add(audio);
        }
        if (name.Contains("Runout"))
        {
            m_runoutSounds.Add(audio);
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
            if (Projectile && m_mesh)
            {
                FTransform transform = m_mesh->GetSocketTransform("Muzzle");
                FActorSpawnParameters SpawnInfo;
                SpawnInfo.Owner = this;
                AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(Projectile, transform, SpawnInfo);
            }
            if (m_fireParticles)
            {
                m_fireParticles->ActivateSystem();
            }
            for (auto& audio : m_fireSounds)
            {
                audio->Play();
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

        for (auto& audio : m_runoutSounds)
        {
            if (0.5f < m_recoil && m_recoil < 1.0f)
            {
                if (!audio->IsPlaying())
                {
                    audio->FadeIn(0.1f);
                }
            }
            else if (audio->IsPlaying())
            {
                audio->FadeOut(0.1f, 0.0f);
            }
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