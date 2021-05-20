// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "HandController.h"

#include "VRPawn.generated.h"

UCLASS()
class INDUVR_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	
	AVRPawn();


protected:

	virtual void BeginPlay() override;

private:

	// Config

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> RightHandControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> LeftHandControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> TeleportHandController;

	// Components

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	// Reference

	UPROPERTY()
	AHandController* LeftHandController;

	UPROPERTY()
	AHandController* RightHandController;


public:

	float GetUserHeight() { return userHeight; }

	UPROPERTY(EditDefaultsOnly)
	float userHeight = 170;

	UFUNCTION()
	void SetStandardControlls();

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	void RightTriggerPressed() { if (RightHandController) RightHandController->TriggerPressed(); }
	void RightTriggerReleased() { if (RightHandController) RightHandController->TriggerReleased(); }

	void LeftTriggerPressed() { if (LeftHandController) LeftHandController->TriggerPressed(); }
	void LeftTriggerReleased() { if (LeftHandController) LeftHandController->TriggerReleased(); }

	void HoveringTools() { toolIndex += 1; selectTool(); }
	void NegativeHoveringTools() { toolIndex -= 1; selectTool(); }

	void selectTool();

	void GrabRightPressed();
	void GrabRightReleased();

	void GrabLeftPressed();
	void GrabLeftReleased();

	UPROPERTY(EditDefaultsOnly)
	int toolIndex = 0;

	UPROPERTY(EditDefaultsOnly)
	int toolMaxIndex = 1;

};