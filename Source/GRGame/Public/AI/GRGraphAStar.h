// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/HexGridNavigationData.h"
#include "Grid/GridData.h"

struct FHexGridPathFilter
{
    FHexGridPathFilter(const AHexGridNavigationData& InNavMeshRef) : NavMeshRef(InNavMeshRef) {}

    /**
     * Used as GetHeuristicCost's multiplier
     */
    float GetHeuristicScale() const;

    /**
     * Estimate of cost from StartNodeRef to EndNodeRef
     */
    float GetHeuristicCost(const int32 StartNodeRef, const int32 EndNodeRef) const;

    /**
     * Real cost of traveling from StartNodeRef directly to EndNodeRef
     */
    float GetTraversalCost(const int32 StartNodeRef, const int32 EndNodeRef) const;

    /**
     * Whether traversing given edge is allowed
     */
    bool IsTraversalAllowed(const int32 NodeA, const int32 NodeB) const;

    /**
     * Whether to accept solutions that do not reach the goal
     */
    bool WantsPartialSolution() const;

protected:

    /**
     * A reference to our NavMesh
     */
    const AHexGridNavigationData& NavMeshRef;
};
