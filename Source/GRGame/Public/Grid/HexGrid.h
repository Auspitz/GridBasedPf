// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/GridData.h"
#include "HexGrid.generated.h"

UCLASS()
class GRGAME_API AHexGrid : public AActor
{
	GENERATED_BODY()
	
public:	

	AHexGrid();

    UFUNCTION(BlueprintCallable, Category = "Hex Grid | Conversions")
    FVector HexToWorld(const FCubeCoord& Coord) const;

    UFUNCTION(BlueprintCallable, Category = "Hex Grid | Conversions")
    FCubeCoord WorldToHex(const FVector& Coord) const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex Grid | Tiles")
    TArray<FHexTile> Tiles;

    UFUNCTION(BlueprintCallable, Category = "Hex Grid | Tiles")
    void SetOccupiedTile(FCubeCoord Coord, AActor* Actor);

    ETileType GenerateTileType() const;

    bool IsTileWalkable(int32 Index) const;

    UFUNCTION(BlueprintCallable, Category = "Hex Grid")
    void CreateGrid(const FHexLayout& Layout, const int32 GridRadius);

    UFUNCTION(BlueprintCallable, Category = "Hex Grid")
    FCubeCoord GetNeighbor(const FCubeCoord& H, const FCubeCoord& Dir);

    UFUNCTION(BlueprintCallable, Category = "Hex Grid")
    FCubeCoord GetDirection(int32 Dir);

    UFUNCTION(BlueprintCallable, Category = "Hex Grid")
    int32 GetDistanceCube(const FCubeCoord& Start, const FCubeCoord& Target) const;

    UFUNCTION(BlueprintCallable, Category = "Hex Grid")
    int32 GetDistance(const FVector& Start, const FVector& Target) const;

private:

    FCubeNeighbors CubeNeighbours;

    FHexLayout CurrentHexLayout;

    TMap<AActor*, FCubeCoord> OccupiedTiles;

};
