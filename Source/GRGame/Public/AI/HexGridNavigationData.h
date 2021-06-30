// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationData.h"
#include "Grid/GridData.h"
#include "Grid/HexGrid.h"
#include "GraphAStar.h"
#include "HexGridNavigationData.generated.h"

/**
 * 
 */
UCLASS()
class GRGAME_API AHexGridNavigationData : public ANavigationData
{
	GENERATED_BODY()

public:

    AHexGridNavigationData();

    //~ Begin AActor
    void BeginPlay() override;
    //~ End

    static FPathFindingResult FindPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query);
	

    // A* Implementation
    using FNodeRef = int32;

    /* Returns number of neighbors that the graph node identified with NodeRef has */
    int32 GetNeighbourCount(FNodeRef NodeRef) const;

    /* Returns whether given node identification is correct */
    bool IsValidRef(FNodeRef NodeRef) const;

    /* Returns neighbor ref */
    FNodeRef GetNeighbour(const FNodeRef NodeRef, const int32 NeiIndex) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    AHexGrid* HexGrid;

private:

    FVector HexOffset;
};
