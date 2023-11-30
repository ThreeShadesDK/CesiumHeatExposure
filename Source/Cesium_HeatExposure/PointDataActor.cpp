// Fill out your copyright notice in the Description page of Project Settings.


#include "PointDataActor.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
APointDataActor::APointDataActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建网格
	DataMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DataMesh"));
	SetRootComponent(DataMesh);

	//创建widget
	DataWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DataWidget"));
	DataWidget->SetupAttachment(GetRootComponent());

	//创建粒子系统
	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticles"));
	NiagaraParticles->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APointDataActor::BeginPlay()
{
	Super::BeginPlay();

	DataWidget->SetVisibility(false);

	CreationTimeRandom = CreationTime + FMath::RandRange(0, 12);
	
}

// Called every frame
void APointDataActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLocation(DeltaTime);

}

void APointDataActor::SetStencilValue(int32 StencilValue) {
	if (DataMesh) {
		DataMesh->SetCustomDepthStencilValue(StencilValue);
	}
}

int32 APointDataActor::GetStencilValue()
{
	if (DataMesh) {
		return DataMesh->CustomDepthStencilValue;
	}
	else {
		return 2;
	}
}

void APointDataActor::SetHeatData(FHeatDataTable InputData) 
{
	Latitude = InputData.Latitude;

	Longitude = InputData.Longitude;

	AirTemperature = InputData.AirTemperature;

	MeanRadiantTemperature = InputData.MeanRadiantTemperature;

	AirSpeed = InputData.AirSpeed;

	RelativeHumidity = InputData.RelativeHumidity;

	MetabolicRate = InputData.MetabolicRate;

	ClothingLevel = InputData.ClothingLevel;

	PMV = InputData.PMV;

	HotFeelings_En = InputData.HotFeelings_En;

	HotFeelings_Cn = InputData.HotFeelings_Cn;

	PPD = InputData.PPD;

	UrbanForm = InputData.UrbanForm;

	TotalPopulationDensity = InputData.TotalPopulationDensity;

	ResidentPopulationDensity = InputData.ResidentPopulationDensity;

	WorkingPopulationDensity = InputData.WorkingPopulationDensity;
}

void APointDataActor::UpdateLocation(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	float RunningTime;
	if (UWorld* MyWorld = GetWorld())
	{
		RunningTime = MyWorld->GetTimeSeconds() - CreationTimeRandom;
	}
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * 6000.f;       //Scale our height by a factor of 20
	SetActorLocation(NewLocation);
}

