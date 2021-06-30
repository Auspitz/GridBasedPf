

#include "AI/GRGraphAStar.h"

float FHexGridPathFilter::GetHeuristicScale() const
{
    return 1.f;
}

float FHexGridPathFilter::GetHeuristicCost(const int32 StartNodeRef, const int32 EndNodeRef) const
{
    return 1.f;
}

float FHexGridPathFilter::GetTraversalCost(const int32 StartNodeRef, const int32 EndNodeRef) const
{
    return 1.f;
}

bool FHexGridPathFilter::IsTraversalAllowed(const int32 NodeA, const int32 NodeB) const
{
    
    return NavMeshRef.HexGrid->IsTileWalkable(NodeB);
}

bool FHexGridPathFilter::WantsPartialSolution() const
{
    return true;
}
