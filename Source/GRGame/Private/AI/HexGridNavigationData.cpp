// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HexGridNavigationData.h"
#include "AI/GRGraphAStar.h"
#include "EngineUtils.h"

AHexGridNavigationData::AHexGridNavigationData()   
{
    HexOffset = FVector(0, 0, 125.f);

    FindPathImplementation = FindPath;
}

void AHexGridNavigationData::BeginPlay()
{
    Super::BeginPlay();
    for(TActorIterator<AHexGrid> It(GetWorld()); It; ++It)
    {
        AHexGrid* Grid = *It;
        if(Grid)
        {
            HexGrid = Grid;
            break;
        }
    }

}

FPathFindingResult AHexGridNavigationData::FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query)
{
    const AHexGridNavigationData* GridNavData = Cast<AHexGridNavigationData>(Query.NavData.Get());
    check(GridNavData != nullptr);

    FPathFindingResult Result(ENavigationQueryResult::Error);

    Result.Path = Query.PathInstanceToFill.IsValid() ? Query.PathInstanceToFill : GridNavData->CreatePathInstance<FNavigationPath>(Query);

    FNavigationPath* NavPath = Result.Path.Get();
    if (NavPath)
    {
        if ((Query.StartLocation - Query.EndLocation).IsNearlyZero())
        {
            Result.Path->GetPathPoints().Reset();
            Result.Path->GetPathPoints().Add(FNavPathPoint(Query.EndLocation));
            Result.Result = ENavigationQueryResult::Success;
        }
        else if (Query.QueryFilter.IsValid())
        {
            FCubeCoord MyPosition = GridNavData->HexGrid->WorldToHex(Query.StartLocation);
            FCubeCoord TargetPosition = GridNavData->HexGrid->WorldToHex(Query.EndLocation);

            const int32 MyPositionIdx = GridNavData->HexGrid->Tiles.IndexOfByKey(MyPosition);
            const int32 TargetPositionIdx = GridNavData->HexGrid->Tiles.IndexOfByKey(TargetPosition);

            FHexGridPathFilter PathFilter(*GridNavData);

            FGraphAStar<AHexGridNavigationData> Pathfinder(*GridNavData);
            //FGraphAStar<AGraphAStarNavMesh> Pathfinder(*GraphAStarNavMesh);

            TArray<int32> Path;
            EGraphAStarResult AStarResult = Pathfinder.FindPath(MyPositionIdx, TargetPositionIdx, PathFilter, Path);
            if (AStarResult == EGraphAStarResult::SearchFail || AStarResult == EGraphAStarResult::InfiniteLoop)
            {
                Result.Result = ENavigationQueryResult::Fail;
                return Result;
            }

            Path.Insert(MyPositionIdx, 0);

            for (const int32& Point : Path)
            {
                //FCubeCoord GridCoord {GridNavData->HexGrid->Tiles[Point]};
                FHexTile GridCoord {GridNavData->HexGrid->Tiles[Point]};

                FNavPathPoint PathPoint;

                if(GridNavData->HexGrid->Tiles.IsValidIndex(Point))
                {
                    PathPoint.Location = GridNavData->HexGrid->HexToWorld(GridCoord.Coord) + GridNavData->HexOffset;
                }
                else
                {
                    PathPoint.Location = GridNavData->HexGrid->HexToWorld(GridCoord.Coord);
                }

                Result.Path->GetPathPoints().Add(FNavPathPoint(PathPoint));
            }

            for (int i = 0; i < Path.Num(); i++)
            {
                //UE_LOG(LogTemp, Warning, TEXT("%s (%s)"), *Path[i].ToString(), *NavPath->GetPathPoints()[i].Location.ToString());
                //DrawDebugSphere(GridNavData->GetWorld(), NavPath->GetPathPoints()[i], 32.f, 6, FColor::Red, false, 5.f, 0, 3.f);
            }
            
            NavPath->MarkReady();
            Result.Result = ENavigationQueryResult::Success;
        }
    
    }


    return Result;
}

int32 AHexGridNavigationData::GetNeighbourCount(FNodeRef NodeRef) const
{
    return 6;
}

bool AHexGridNavigationData::IsValidRef(FNodeRef NodeRef) const
{
    return HexGrid->Tiles.IsValidIndex(NodeRef);
}

AHexGridNavigationData::FNodeRef AHexGridNavigationData::GetNeighbour(const FNodeRef NodeRef, const int32 NeiIndex) const
{
    FCubeCoord Neigh{ HexGrid->GetNeighbor(HexGrid->Tiles[NodeRef].Coord, HexGrid->GetDirection(NeiIndex)) };
    return HexGrid->Tiles.IndexOfByKey(Neigh);
}
