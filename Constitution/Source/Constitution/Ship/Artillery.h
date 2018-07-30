// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "Artillery.generated.h"

UCLASS()
class CONSTITUTION_API AArtillery : public AActor
{
	GENERATED_BODY()

public:
	AArtillery();

private:
    USkinnedMeshComponent* m_mesh = nullptr;
    UParticleSystemComponent* m_fireParticles = nullptr;
    TArray<UAudioComponent*> m_fireSounds;
    TArray<UAudioComponent*> m_runoutSounds;

    bool m_firing = false;
    bool m_firedShot = false;
    float m_fireTime = 0;
    float m_reloadDuration = 0;
    float m_runoutDuration = 0;
    float m_recoil = 0;

protected:
    UPROPERTY(EditAnywhere, Category = "Firing")
    TSubclassOf<AProjectile> Projectile = nullptr;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float FireTimeVariance = 0.5;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.0", ClampMax = "5.0", UIMin = "0.0", UIMax = "5.0"))
    float RecoilTime = 1.0;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.0", ClampMax = "60.0", UIMin = "0.0", UIMax = "60.0"))
    float ReloadTime = 10.0;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.0", ClampMax = "5.0", UIMin = "0.0", UIMax = "5.0"))
    float ReloadVariance = 1.0;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
    float RunoutTime = 5.0;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
    float RunoutVariance = 2.0;

	virtual void BeginPlay() override; 
    
    UFUNCTION(BlueprintNativeEvent)
    void SetRecoil(float recoil);
    virtual void SetRecoil_Implementation(float recoil);

public:
	virtual void Tick(float DeltaTime) override;
    void Fire(float delay);
    bool CanFire();
};
