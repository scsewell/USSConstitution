// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Artillery.h"
#include "Ship.generated.h"

UCLASS()
class CONSTITUTION_API AShip : public AActor
{
	GENERATED_BODY()

public:
	AShip();

private:
    struct Artillery
    {
        AArtillery* artillery = nullptr;
        FVector position;
    };

    TArray<Artillery> m_leftArtillery;
    TArray<Artillery> m_rightArtillery;

    UFUNCTION(BlueprintCallable)
    void FireCannons();
    void FireCannons(TArray<Artillery>* artillery);

protected:
    UPROPERTY(EditAnywhere, Category = "Firing")
    bool AutoFire = false;

    UPROPERTY(EditAnywhere, Category = "Firing", meta = (ClampMin = "0.1", ClampMax = "50.0", UIMin = "0.1", UIMax = "50.0"))
    float RollingFireSpeed = 5.0;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void ToggleAutoFire();
};
