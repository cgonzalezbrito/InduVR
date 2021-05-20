// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportHandController.h"

#include "NavigationSystem.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Pawn.h"

#include "VRPawn.h"

ATeleportHandController::ATeleportHandController()
{
	//PrimaryActorTick.bCanEverTick = true;

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
	DestinationMarker->SetupAttachment(GetRootComponent());

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
	TeleportPath->SetupAttachment(GetRootComponent());
}

void ATeleportHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDestinationMarker();
}

void ATeleportHandController::UpdateDestinationMarker()
{
	TArray<FVector> Path;
	FVector Location;

	bHasDestination = FindTeleportDestination(Path, Location);

	if (bHasDestination)
	{
		DestinationMarker->SetVisibility(true);
		DestinationMarker->SetWorldLocation(Location);

		DrawTeleportPath(Path);
	}
	else
	{
		DestinationMarker->SetVisibility(false);

		TArray<FVector> EmptyPath;
		DrawTeleportPath(EmptyPath);
	}
}

bool ATeleportHandController::FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation)
{
	FVector Start = GetActorLocation();
	FVector Look = GetActorForwardVector();

	FPredictProjectilePathParams Params(
		TeleportProjectileRadius,
		Start,
		Look * TeleportProjectileSpeed,
		TeleportSimulationTime,
		ECollisionChannel::ECC_Visibility,
		this
	);
	Params.bTraceComplex = true;
	FPredictProjectilePathResult Result;

	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, Result);

	if (!bHit) return false;

	for (FPredictProjectilePathPointData PointData : Result.PathData)
	{
		OutPath.Add(PointData.Location);
	}

	FNavLocation NavLocation;
	bool bOnNavMesh = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(Result.HitResult.Location, NavLocation, TeleportProjectionExtend);

	if (!bOnNavMesh) return false;

	OutLocation = NavLocation.Location;

	return true;
}

void ATeleportHandController::DrawTeleportPath(const TArray<FVector>& Path)
{
	UpdateSpline(Path);

	for (USplineMeshComponent* SPlineMesh : TeleportPathMeshPool)
	{
		SPlineMesh->SetVisibility(false);
	}

	int32 SegmentNum = Path.Num() - 1;

	for (int32 i = 0; i < SegmentNum; ++i)
	{
		if (TeleportPathMeshPool.Num() <= i)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(TeleportArchMesh);
			SplineMesh->SetMaterial(0, TeleportArchMaterial);
			SplineMesh->RegisterComponent();

			TeleportPathMeshPool.Add(SplineMesh);
		}

		USplineMeshComponent* SplineMesh = TeleportPathMeshPool[i];
		SplineMesh->SetVisibility(true);

		FVector StartPos, StartTangent, EndPos, EndTangent;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	}
}

void ATeleportHandController::UpdateSpline(const TArray<FVector>& Path)
{
	TeleportPath->ClearSplinePoints(false);

	for (int32 i = 0; i < Path.Num(); i++)
	{
		FVector LocalPosition = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
		FSplinePoint Point(i, LocalPosition, ESplinePointType::Curve);
		TeleportPath->AddPoint(Point, false);
	}

	TeleportPath->UpdateSpline();
}

void ATeleportHandController::TriggerPressed()
{
	if (bHasDestination) BeginTeleport();
}

void ATeleportHandController::BeginTeleport()
{
	StartFade(0, 1);

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ATeleportHandController::FinishTeleport, teleportFadeTime);
}

void ATeleportHandController::FinishTeleport()
{
	AVRPawn* WorldPawn = Cast<AVRPawn>(GetAttachParentActor());

	if (WorldPawn != nullptr)
	{
		FVector Destination = DestinationMarker->GetComponentLocation();
		Destination.Z += WorldPawn->GetUserHeight();
		WorldPawn->SetActorLocation(Destination);
	}

	StartFade(1, 0);
}

void ATeleportHandController::StartFade(float fromAlpha, float toAlpha)
{
	APawn* Pawn = Cast<APawn>(GetAttachParentActor());

	if (Pawn != nullptr)
	{
		APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
		if (PC != nullptr)
		{
			PC->PlayerCameraManager->StartCameraFade(fromAlpha, toAlpha, teleportFadeTime, FLinearColor::Black);
		}
	}
}

