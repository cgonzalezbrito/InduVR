// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandController.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"

#include "TeleportHandController.generated.h"

/**
 * 
 */
UCLASS()
class INDUVR_API ATeleportHandController : public AHandController
{
	GENERATED_BODY()

public:

	ATeleportHandController();

	virtual void Tick(float DeltaTime) override;

	void TriggerPressed() override;

private:

	// Functions 

	void UpdateDestinationMarker();
	bool FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation);
	void DrawTeleportPath(const TArray<FVector>& Path);
	void UpdateSpline(const TArray<FVector>& Path);

	void  BeginTeleport();
	void  FinishTeleport();

	void StartFade(float fromAlpha, float toAlpha);

	bool bHasDestination;

	// Components

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* TeleportPath;

	UPROPERTY()
	TArray<class USplineMeshComponent*> TeleportPathMeshPool;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;

	// Configuration Parameters

	UPROPERTY(EditAnywhere)
	float TeleportProjectileRadius = 10;

	UPROPERTY(EditAnywhere)
	float TeleportProjectileSpeed = 800;

	UPROPERTY(EditAnywhere)
	float TeleportSimulationTime = 1;

	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtend = FVector(100, 100, 100);

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* TeleportArchMesh;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TeleportArchMaterial;

	UPROPERTY(EditAnywhere)
	float teleportFadeTime = 0.5;
	
};
