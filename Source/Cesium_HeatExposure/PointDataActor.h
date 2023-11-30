// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PointDataActor.generated.h"

USTRUCT(BlueprintType)
struct FHeatDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Latitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Longitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeanRadiantTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RelativeHumidity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MetabolicRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClothingLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PMV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HotFeelings_En;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HotFeelings_Cn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PPD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UrbanForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalPopulationDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResidentPopulationDensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WorkingPopulationDensity;
};



UCLASS()
class CESIUM_HEATEXPOSURE_API APointDataActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointDataActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStencilValue(int32 StencilValue);

	int32 GetStencilValue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetHeatData(FHeatDataTable InputData);

	void UpdateLocation(float DeltaTime);


private:
	//表示数据的网格体
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"));
	UStaticMeshComponent* DataMesh;

	//显示数据的UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* DataWidget;

	//粒子系统
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* NiagaraParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float Latitude;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float Longitude;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float AirTemperature;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float MeanRadiantTemperature;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float AirSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float RelativeHumidity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	int32 MetabolicRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float ClothingLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float PMV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	FString HotFeelings_En;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	FString HotFeelings_Cn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float PPD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	FString UrbanForm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float TotalPopulationDensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float ResidentPopulationDensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float WorkingPopulationDensity;

	float CreationTimeRandom;

public:	
	FORCEINLINE UWidgetComponent* GetDataWidget() const { return DataWidget; }
};
