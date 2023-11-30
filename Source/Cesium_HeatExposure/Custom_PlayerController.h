// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Custom_PlayerController.generated.h"


UCLASS()
class CESIUM_HEATEXPOSURE_API ACustom_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACustom_PlayerController();
protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "True"))
		TSubclassOf<class UUserWidget> CursorWidgetClass;

	UUserWidget* CursorWidget;

	//reference  to the overall HUD Overlay Blueprint Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "True"))
	TSubclassOf<class UUserWidget> HUDOverlayClass;

	//variable to hold the HUD overlay widget after creating it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "True"))
	UUserWidget* HUDOverlay;

	
};
