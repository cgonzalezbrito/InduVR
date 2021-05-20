// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"

#include "HandController.generated.h"

UCLASS()
class INDUVR_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	

	AHandController();

	void SetHand(EControllerHand Hand);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HandController")
	void setHandEvent(EControllerHand Hand);

	virtual void TriggerPressed() {}
	virtual void TriggerReleased() {}

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HandController")
	void GripPressedEvent();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HandController")
	void GripReleaseEvent();

private:
	// Components
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;

};
