// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"

// Sets default values
AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->SetShowDeviceModel(true);

}

void AHandController::SetHand(EControllerHand Hand)
{
	MotionController->SetTrackingSource(Hand);
}
