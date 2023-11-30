// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyPawn::AMyPawn() :
	TurnRate(30.f),
	LookUpRate(30.f),
	ZoomRate(30.f),
	MinLength(1000.f),
	MaxLength(10000.f),
	DefaultLength(50000),
	bRightMouseButtonPressed(false),
	LineTraceMaxLength(5000000.f),
	EnabledPointActor_1(nullptr),
	EnabledPointActor_2(nullptr),
	EnabledPointActor_3(nullptr),
	LastStencilValue1(2),
	LastStencilValue2(2),
	LastStencilValue3(2)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a follow camera
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	SetRootComponent(PawnCamera);
	//PawnCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);//attach camera to end of boom
	PawnCamera->bUsePawnControlRotation = true;//Camera does not rotate relative to arm

	//do not rotate along with controller
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		//PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}


	//旋转角色到指定方向
	RotateToFaceCenter();

	CurrentLength = DefaultLength;
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateToFaceCenter();
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AMyPawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPawn::LookUp);
	PlayerInputComponent->BindAxis("MouseWheel", this, &AMyPawn::ZoomIn);

	PlayerInputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AMyPawn::RightMouseButtonPressed);
	PlayerInputComponent->BindAction("RightMouseButton", IE_Released, this, &AMyPawn::RightMouseButtonReleased);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AMyPawn::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction("CtrlLeftMouseButton", IE_Pressed, this, &AMyPawn::CtrlLeftMouseButtonPressed);

}

void AMyPawn::RightMouseButtonPressed()
{
	bRightMouseButtonPressed = true;
}

void AMyPawn::RightMouseButtonReleased()
{
	bRightMouseButtonPressed = false;
}

void AMyPawn::Turn(float Rate)
{
	if (bRightMouseButtonPressed) {

		//AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());

		FVector actorLocation = GetActorLocation();
		FVector rotationAxis = FVector(0.0f, 0.0f, 1.0f); // 旋转轴向，这里选择Z轴

		float RotationValue = TurnRate * Rate * GetWorld()->GetDeltaSeconds();
		//围绕中心点水平旋转后的坐标位置
		FVector rotatedLocation = (actorLocation - CenterLocation).RotateAngleAxis(RotationValue, rotationAxis) + CenterLocation;

		SetActorLocation(rotatedLocation);
		//面朝中心
		//RotateToFaceCenter();
	}
}

void AMyPawn::LookUp(float Rate)
{
	if (bRightMouseButtonPressed) {

		FRotator PawnCameraRotation = PawnCamera->GetComponentRotation();
		FVector PawnLocation = GetActorLocation();
		FVector Pawn2Center = CenterLocation - PawnLocation;
		FVector rotationAxis = FRotationMatrix(Pawn2Center.Rotation()).GetUnitAxis(EAxis::Y);// 旋转轴

		float RotationValue = LookUpRate * Rate * GetWorld()->GetDeltaSeconds();
		FVector rotatedVector = (-Pawn2Center).RotateAngleAxis(RotationValue, rotationAxis);
		//假如俯仰角过大，则返回
		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, -1, FColor::Red, FString::Printf(TEXT("rotatedVector.Rotation().Pitch,%f"), rotatedVector.Rotation().Pitch));
		if (rotatedVector.Rotation().Pitch > 89.9f) {
			rotatedVector = (-Pawn2Center).RotateAngleAxis(89.9f-(-Pawn2Center).Rotation().Pitch, rotationAxis);
		}
		if (FMath::Abs( rotatedVector.Rotation().Yaw- VectorYawLastFrame)>150.f) {//若处于正上方则采用上一帧的位置
			VectorYawLastFrame = (- Pawn2Center).Rotation().Yaw;
			return;
		}

		VectorYawLastFrame = rotatedVector.Rotation().Yaw;
		FVector rotatedLocation = rotatedVector + CenterLocation;
		//不允许转到地下去了
		rotatedLocation.Z = rotatedLocation.Z < MinCameraHeight ? MinCameraHeight : rotatedLocation.Z;
		SetActorLocation(rotatedLocation);
	}
}

void AMyPawn::ZoomIn(float Rate)
{
	float DeltaZoom = Rate * ZoomRate * GetWorld()->GetDeltaSeconds();
	FVector PawnLocation = GetActorLocation();
	FVector Pawn2Center = CenterLocation - PawnLocation;
	Pawn2Center.Normalize();
	//距离测量
	CurrentLength += DeltaZoom;
	CurrentLength = FMath::Clamp(CurrentLength, MinLength, MaxLength);

	FVector NewLocation = CenterLocation - Pawn2Center * CurrentLength;

	//不允许放缩到地下去了
	NewLocation.Z = NewLocation.Z < MinCameraHeight ? MinCameraHeight : NewLocation.Z;
	SetActorLocation(NewLocation);
}

void AMyPawn::RotateToFaceCenter() {
	FVector PawnLocation = GetActorLocation();
	FVector Pawn2Center = CenterLocation - PawnLocation;
	FRotator TargetRotation = Pawn2Center.Rotation();

	if (PlayerController) {
		CamerawYawLastFrame = TargetRotation.Yaw;
		PlayerController->SetControlRotation(TargetRotation);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(2, -1, FColor::Red, FString::Printf(TEXT("TargetRotation,%f,%f,%f"), TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll));
	}
}

void AMyPawn::LeftMouseButtonPressed() {	
	//假如三个位置都已激活,则返回
	if (EnabledPointActor_1 && EnabledPointActor_2 && EnabledPointActor_3) {
		return;
	}
	FHitResult TraceResult;
	TraceUnderMouse(TraceResult);
	if (TraceResult.bBlockingHit) {
		TraceHitObj = Cast<APointDataActor>(TraceResult.Actor);
		if (TraceHitObj && TraceHitObj->GetDataWidget()) {
			//
			if (EnabledPointActor_1 == nullptr&& TraceHitObj!= EnabledPointActor_2&& TraceHitObj!= EnabledPointActor_3) {
				LastStencilValue1 = TraceHitObj->GetStencilValue();
				TraceHitObj->SetStencilValue(3);
				EnabledPointActor_1 = TraceHitObj;
				PointDataDelegate_1.Broadcast(EnabledPointActor_1);
				UE_LOG(LogTemp, Warning, TEXT("1"));
				//if (GEngine) { GEngine->AddOnScreenDebugMessage(3, -1, FColor::Red, FString::Printf(TEXT("1"))); }
				return;
			}
			else if (EnabledPointActor_2 == nullptr && TraceHitObj != EnabledPointActor_1 && TraceHitObj != EnabledPointActor_3) {
				LastStencilValue2 = TraceHitObj->GetStencilValue();
				TraceHitObj->SetStencilValue(3);
				EnabledPointActor_2 = TraceHitObj;
				PointDataDelegate_2.Broadcast(EnabledPointActor_2);
				UE_LOG(LogTemp, Warning, TEXT("2"));
				return;
			}
			else if (EnabledPointActor_3 == nullptr && TraceHitObj != EnabledPointActor_1 && TraceHitObj != EnabledPointActor_2) {
				LastStencilValue3 = TraceHitObj->GetStencilValue();
				TraceHitObj->SetStencilValue(3);
				EnabledPointActor_3 = TraceHitObj;
				PointDataDelegate_3.Broadcast(EnabledPointActor_3);
				UE_LOG(LogTemp, Warning, TEXT("3"));
				return;
			}
			//if (LastTraceHitObj && LastTraceHitObj->GetDataWidget()) {
				//LastTraceHitObj->GetDataWidget()->SetVisibility(false);
				//if (LastTraceHitObj && LastTraceHitObj->GetDataWidget()) {
					//LastTraceHitObj->SetStencilValue(1);
				//}
			//}
			//LastTraceHitObj = TraceHitObj;
		}
	}
}

void AMyPawn::CtrlLeftMouseButtonPressed() {
	FHitResult TraceResult;
	TraceUnderMouse(TraceResult);
	if (TraceResult.bBlockingHit) {
		TraceHitObj = Cast<APointDataActor>(TraceResult.Actor);
		if (TraceHitObj && TraceHitObj->GetDataWidget()) {
			//if (GEngine) { GEngine->AddOnScreenDebugMessage(3, -1, FColor::Red, FString::Printf(TEXT("3"))); }
			if (EnabledPointActor_1 == TraceHitObj) {
				TraceHitObj->SetStencilValue(LastStencilValue1);
				EnabledPointActor_1 = nullptr;;
				PointDataDelegate_1.Broadcast(nullptr);
				return;
			}
			else if (EnabledPointActor_2 == TraceHitObj) {
				TraceHitObj->SetStencilValue(LastStencilValue2);
				EnabledPointActor_2 = nullptr;
				PointDataDelegate_2.Broadcast(nullptr);
				return;
			}
			else if (EnabledPointActor_3 == TraceHitObj) {
				TraceHitObj->SetStencilValue(LastStencilValue3);
				EnabledPointActor_3 = nullptr;
				PointDataDelegate_3.Broadcast(nullptr);
				return;
			}
			//if (LastTraceHitObj && LastTraceHitObj->GetDataWidget()) {
				//LastTraceHitObj->GetDataWidget()->SetVisibility(false);
				//if (LastTraceHitObj && LastTraceHitObj->GetDataWidget()) {
					//LastTraceHitObj->SetStencilValue(1);
				//}
			//}
			//LastTraceHitObj = TraceHitObj;
		}
	}
}

bool AMyPawn::TraceUnderMouse(FHitResult& OutHitResult) {
	FVector2D MousePosition;
	FVector MouseWorldPositon;
	FVector MouseWorldDirection;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	//get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(PlayerController,
		MousePosition, MouseWorldPositon, MouseWorldDirection);
	if (bScreenToWorld) {
		const FVector Start = MouseWorldPositon;//MuzzleSocketLocation;
		const FVector End = MouseWorldPositon + MouseWorldDirection * LineTraceMaxLength;
		//trace outward from crosshairs world location
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit) {//if hit object 
			return true;
		}
	}
	return false;
}

