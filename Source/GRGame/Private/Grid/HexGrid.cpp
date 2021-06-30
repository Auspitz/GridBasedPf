// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/HexGrid.h"

AHexGrid::AHexGrid()
{
	PrimaryActorTick.bCanEverTick = true;

    // Hardcoded values for now
    CurrentHexLayout.Size = FVector(100.f, 100.f, 0);
    CurrentHexLayout.Origin = FVector(0);
}

FVector AHexGrid::HexToWorld(const FCubeCoord& Coord) const
{
    const FHexTileOrientation& M = CurrentHexLayout.Orientation;
    double X = (M.f0 * Coord.q + M.f1 * Coord.r) * CurrentHexLayout.Size.X;
    double Y = (M.f2 * Coord.q + M.f3 * Coord.r) * CurrentHexLayout.Size.Y;

    return FVector(X + CurrentHexLayout.Origin.X, Y + CurrentHexLayout.Origin.Y, CurrentHexLayout.Origin.Z);
}

FCubeCoord AHexGrid::WorldToHex(const FVector& Coord) const
{
    const FHexTileOrientation& M = CurrentHexLayout.Orientation;
    FVector Vec = FVector((Coord.X - CurrentHexLayout.Origin.X) / CurrentHexLayout.Size.X,
                          (Coord.Y - CurrentHexLayout.Origin.Y) / CurrentHexLayout.Size.Y,
                          Coord.Z);

    double q = M.b0 * Vec.X + M.b1 * Vec.Y;
    double r = M.b2 * Vec.X + M.b3 * Vec.Y;

    int32 q_round = FMath::RoundToInt(q);
    int32 r_round = FMath::RoundToInt(r);
    return FCubeCoord(q_round, r_round, -q_round - r_round);
}

void AHexGrid::SetOccupiedTile(FCubeCoord Coord, AActor* Actor)
{
    if(!OccupiedTiles.Contains(Actor))
    {
        OccupiedTiles.Add(Actor, Coord);
    }
}

ETileType AHexGrid::GenerateTileType() const
{
    // Ground, Water, Void, Wall
    TArray<int32> WeightedTileTypes = {100, 10, 2, 15};

    int32 TotalWeight = 0;
    for(int32 Val : WeightedTileTypes)
    {
        TotalWeight += Val;
    }

    int32 RandomValue = FMath::RandHelper(TotalWeight);
    
    for(int32 i = 0; i < WeightedTileTypes.Num(); i++)
    {
        if(RandomValue < WeightedTileTypes[i])
        {
            return ETileType(i);
        }
        RandomValue -= WeightedTileTypes[i];
    }


    return ETileType::Ground;
}

bool AHexGrid::IsTileWalkable(int32 Index) const
{
    if(Tiles.IsValidIndex(Index))
    {
        FHexTile Tile = Tiles[Index];
        return Tile.TileType == ETileType::Ground ? true : false;
    }
    return false;
}

void AHexGrid::CreateGrid(const FHexLayout& Layout, const int32 GridRadius)
{
    CurrentHexLayout = Layout;
    for(int32 X = -GridRadius; X <= GridRadius; X++)
    {
        for(int32 Y = FMath::Max(-GridRadius, -X - GridRadius); Y <= FMath::Min(GridRadius, -X + GridRadius); Y++)
        {
            int32 Z = -X-Y;
            FCubeCoord Coord{X, Y, Z};
            FHexTile HexTile(Coord, GenerateTileType());
            Tiles.Add(HexTile);
        }
    }
    
    
}

FCubeCoord AHexGrid::GetNeighbor(const FCubeCoord& H, const FCubeCoord& Dir)
{
    return H + Dir;
}

FCubeCoord AHexGrid::GetDirection(int32 Dir)
{
    return CubeNeighbours.GetDirection(Dir);
}

int32 AHexGrid::GetDistanceCube(const FCubeCoord& Start, const FCubeCoord& Target) const
{
    return (FMath::Abs(Start.q - Target.q) + FMath::Abs(Start.r - Target.r) + FMath::Abs(Start.s - Target.s)) / 2;
}

int32 AHexGrid::GetDistance(const FVector& Start, const FVector& Target) const
{
    FCubeCoord StartCube = WorldToHex(Start);
    FCubeCoord TargetCube = WorldToHex(Target);
    return GetDistanceCube(StartCube, TargetCube);
}

