// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PointDataActor.h"
#include "MyPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointDataDelegate, APointDataActor*, PointDataActor);

UCLASS()
class CESIUM_HEATEXPOSURE_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RightMouseButtonPressed();

	void RightMouseButtonReleased();

	void LeftMouseButtonPressed();

	void CtrlLeftMouseButtonPressed();

	void Turn(float Rate);

	void LookUp(float Rate);

	void ZoomIn(float Rate);

	void RotateToFaceCenter();

	bool TraceUnderMouse(FHitResult& OutHitResult);


private:
	//Camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* PawnCamera;

	bool bRightMouseButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float TurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ZoomRate;

	UPROPERTY()
	float CameraCurrentFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MaxLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MinLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DefaultLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MinCameraHeight;

	UPROPERTY()
	float CurrentLength;

	UPROPERTY()
	APlayerController* PlayerController;

	//视角旋转中心的位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector CenterLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	APointDataActor* TraceHitObj;

	APointDataActor* LastTraceHitObj;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LineTraceMaxLength;

	FVector CamerawLocationLastFrame;

	float CamerawYawLastFrame;

	float VectorYawLastFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ClickOn, meta = (AllowPrivateAccess = "True"))
	APointDataActor* EnabledPointActor_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ClickOn, meta = (AllowPrivateAccess = "True"))
	APointDataActor* EnabledPointActor_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ClickOn, meta = (AllowPrivateAccess = "True"))
	APointDataActor* EnabledPointActor_3;

	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
	FPointDataDelegate PointDataDelegate_1;

	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
	FPointDataDelegate PointDataDelegate_2;

	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
	FPointDataDelegate PointDataDelegate_3;

	int32 LastStencilValue1;

	int32 LastStencilValue2;

	int32 LastStencilValue3;

public:	
	FORCEINLINE UCameraComponent* GetPawnCamera() const { return PawnCamera; }

};
