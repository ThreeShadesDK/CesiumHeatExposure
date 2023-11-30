// Fill out your copyright notice in the Description page of Project Settings.


#include "Custom_PlayerController.h"
#include "Blueprint/UserWidget.h"

ACustom_PlayerController::ACustom_PlayerController() {
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


}

void ACustom_PlayerController::OnConstruction(const FTransform& Transform) {

}

void ACustom_PlayerController::BeginPlay() 
{
	Super::BeginPlay();

    //check our HUD overlay class TSubofclass variable
    if (HUDOverlayClass) {
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
        if (HUDOverlay) {
            HUDOverlay->AddToViewport();
            HUDOverlay->SetVisibility(ESlateVisibility::Visible);
        }
    }

    if (CursorWidgetClass)
    {
        CursorWidget = CreateWidget<UUserWidget>(this, CursorWidgetClass);
        if (CursorWidget) {
            SetMouseCursorWidget(EMouseCursor::Default, CursorWidget);
        }
    }
}

