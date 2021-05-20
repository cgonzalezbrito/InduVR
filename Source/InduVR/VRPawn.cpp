// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"

// Sets default values
AVRPawn::AVRPawn()
{
	//PrimaryActorTick.bCanEverTick = false;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	SetRootComponent(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (LeftHandControllerClass)
	{
		LeftHandController = GetWorld()->SpawnActor<AHandController>(LeftHandControllerClass);
		LeftHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		LeftHandController->setHandEvent(EControllerHand::Left);
		LeftHandController->SetHand(EControllerHand::Left);
		LeftHandController->SetOwner(this);
	}

	if (RightHandControllerClass) {
		RightHandController = GetWorld()->SpawnActor<AHandController>(RightHandControllerClass);
		RightHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		RightHandController->SetHand(EControllerHand::Right);
		RightHandController->setHandEvent(EControllerHand::Right);
		RightHandController->SetOwner(this);
	}
	
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PrimaryActorTick.bCanEverTick = true;

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Pressed, this, &AVRPawn::RightTriggerPressed);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Released, this, &AVRPawn::RightTriggerReleased);

	PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Pressed, this, &AVRPawn::LeftTriggerPressed);
	PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Released, this, &AVRPawn::LeftTriggerReleased);

	PlayerInputComponent->BindAction(TEXT("HoveringTools"), IE_Pressed, this, &AVRPawn::HoveringTools);
	PlayerInputComponent->BindAction(TEXT("NegativeHoveringTools"), IE_Pressed, this, &AVRPawn::NegativeHoveringTools);

	PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Pressed, this, &AVRPawn::GrabLeftPressed);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), IE_Released, this, &AVRPawn::GrabLeftReleased);

	PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Pressed, this, &AVRPawn::GrabRightPressed);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), IE_Released, this, &AVRPawn::GrabRightReleased);

}

void AVRPawn::SetStandardControlls()
{
	GetWorld()->DestroyActor(LeftHandController);
	LeftHandController = GetWorld()->SpawnActor<AHandController>(LeftHandControllerClass);
	LeftHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	LeftHandController->SetHand(EControllerHand::Left);
	LeftHandController->setHandEvent(EControllerHand::Left);
	LeftHandController->SetOwner(this);

	GetWorld()->DestroyActor(RightHandController);
	RightHandController = GetWorld()->SpawnActor<AHandController>(RightHandControllerClass);
	RightHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	RightHandController->SetHand(EControllerHand::Right);
	RightHandController->setHandEvent(EControllerHand::Right);
	RightHandController->SetOwner(this);
}

void AVRPawn::selectTool()
{
	if (toolIndex > toolMaxIndex) toolIndex = 0;
	if (toolIndex < 0) toolIndex = toolMaxIndex;

	TSubclassOf<AHandController> AuxiliarLeftHandControllerClass;
	TSubclassOf<AHandController> AuxiliarRightHandControllerClass;

	switch (toolIndex)
	{
	case 0:
		AuxiliarLeftHandControllerClass = LeftHandControllerClass;
		AuxiliarRightHandControllerClass = RightHandControllerClass;
		break;
	case 1:
		AuxiliarLeftHandControllerClass = LeftHandControllerClass;
		AuxiliarRightHandControllerClass = TeleportHandController;
		break;

	default:
		AuxiliarLeftHandControllerClass = LeftHandControllerClass;
		AuxiliarRightHandControllerClass = RightHandControllerClass;
		break;
	}

	if (AuxiliarLeftHandControllerClass)
	{
		GetWorld()->DestroyActor(LeftHandController);
		LeftHandController = GetWorld()->SpawnActor<AHandController>(AuxiliarLeftHandControllerClass);
		LeftHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		LeftHandController->SetHand(EControllerHand::Left);
		if (toolIndex != 2) LeftHandController->setHandEvent(EControllerHand::Left);
		LeftHandController->SetOwner(this);
	}

	if (AuxiliarRightHandControllerClass) {
		GetWorld()->DestroyActor(RightHandController);
		RightHandController = GetWorld()->SpawnActor<AHandController>(AuxiliarRightHandControllerClass);
		RightHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		RightHandController->SetHand(EControllerHand::Right);
		if (toolIndex != 2) RightHandController->setHandEvent(EControllerHand::Right);
		RightHandController->SetOwner(this);
	}
}

void AVRPawn::GrabRightPressed()
{
	if (toolIndex == 0)
	{
		if (RightHandController) { RightHandController->GripPressedEvent(); }
	}
}

void AVRPawn::GrabRightReleased()
{
	if (toolIndex == 0)
	{
		if (RightHandController) { RightHandController->GripReleaseEvent(); }
	}
}

void AVRPawn::GrabLeftPressed()
{
	if (toolIndex == 0)
	{
		if (LeftHandController) { LeftHandController->GripPressedEvent(); }
	}
}

void AVRPawn::GrabLeftReleased()
{
	if (toolIndex == 0)
	{
		if (LeftHandController) { LeftHandController->GripReleaseEvent(); }
	}
}