// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridData.generated.h"

//USTRUCT(BlueprintType, meta = (HasNativeMake="GridData.MakeCubeCoord", HasNativeBreak="GridData.BreakCubeCoord"))
USTRUCT(BlueprintType)
struct FCubeCoord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 q;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 r;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 s;

    FCubeCoord() 
    {
        q = 0;
        r = 0;
        s = 0;

    };

    FCubeCoord(int32 q_, int32 r_, int32 s_)
     : q(q_), r(r_), s(s_)
    {
        check(q + r + s == 0);
    }

    friend bool operator == (FCubeCoord Lhs, FCubeCoord Rhs)
    {
        return (Lhs.q == Rhs.q && Lhs.r == Rhs.r && Lhs.s == Rhs.s);
    }

    friend bool operator != (FCubeCoord Lhs, FCubeCoord Rhs)
    {
        return !(Lhs == Rhs);
    }

    friend FCubeCoord operator + (FCubeCoord Lhs, FCubeCoord Rhs)
    {
        return FCubeCoord {Lhs.q + Rhs.q, Lhs.r + Rhs.r, Lhs.s + Rhs.s};
    }

    friend FCubeCoord operator - (FCubeCoord Lhs, FCubeCoord Rhs)
    {
        return FCubeCoord { Lhs.q - Rhs.q, Lhs.r - Rhs.r, Lhs.s - Rhs.s };
    }

    friend FCubeCoord operator * (FCubeCoord Coord, int32 Scalar)
    {
        return FCubeCoord { Coord.q * Scalar, Coord.r * Scalar, Coord.s * Scalar };
    }

    int32 Length(FCubeCoord Coord)
    {
        return ((FMath::Abs(Coord.q) + FMath::Abs(Coord.r) + FMath::Abs(Coord.s)) / 2);
    }

    int32 Distance(FCubeCoord Lhs, FCubeCoord Rhs)
    {
        return Length(Rhs - Lhs);
    }

    FCubeCoord MakeCubeCoord(int32 q_, int32 r_, int32 s_)
    {
        return FCubeCoord(q_, r_, s_);
    }

    void BreakCubeCoord(FCubeCoord InCube, int32& q_, int32& r_, int32& s_)
    {
        q_ = InCube.q;
        r_ = InCube.r;
        s_ = InCube.s;
    }



};



USTRUCT(BlueprintType)
struct FCubeNeighbors
{
    
    GENERATED_BODY()

    TArray<FCubeCoord> Directions = { 
        FCubeCoord(1, 0, -1), FCubeCoord(1, -1, 0), FCubeCoord(0, -1, 1),
        FCubeCoord(-1, 0, 1), FCubeCoord(-1, 1, 0), FCubeCoord(0, 1, -1)
    };

    FCubeCoord GetDirection(int32 Direction)
    {
        check(0 <= Direction && Direction < 6);
        return Directions[Direction];
    }

    FCubeCoord GetNeighbour(FCubeCoord Coord, int32 Direction)
    {
        return FCubeCoord {Coord + GetDirection(Direction)};
    }

};

struct FHexTileOrientation
{
    double f0, f1, f2, f3;
    double b0, b1, b2, b3;
    double start_angle;

    FHexTileOrientation() {};

    FHexTileOrientation(double f0_, double f1_, double f2_, double f3_,
                    double b0_, double b1_, double b2_, double b3_,
                    double start_angle_)
                    : f0(f0_), f1(f1_), f2(f2_), f3(f3_),
                    b0(b0_), b1(b1_), b2(b2_), b3(b3_),
                    start_angle(start_angle_) {}
};

namespace HexLayoutType
{
    static const FHexTileOrientation Layout_Pointy =
        FHexTileOrientation(FMath::Sqrt(3.0), FMath::Sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
            FMath::Sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
            0.5);

    static const FHexTileOrientation Layout_Flat =
        FHexTileOrientation(3.0 / 2.0, 0.0, FMath::Sqrt(3.0) / 2.0, FMath::Sqrt(3.0),
            2.0 / 3.0, 0.0, -1.0 / 3.0, FMath::Sqrt(3.0) / 3.0,
            0.0);
}

USTRUCT(BlueprintType)
struct FHexLayout 
{
    GENERATED_BODY()

    FHexLayout() {};

    FHexTileOrientation Orientation = HexLayoutType::Layout_Flat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Size;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Origin;

    FHexLayout(FHexTileOrientation Orientation_, FVector Size_, FVector Origin_)
        : Orientation(Orientation_), Size(Size_), Origin(Origin_) {}
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
    Ground,
    Water,
    Void,
    Wall
};

UENUM(BlueprintType)
enum class ETileRotation : uint8
{
    N,
    NE,
    SE,
    S,
    SW,
    NW
};


USTRUCT(BlueprintType)
struct FHexTile
{
    GENERATED_BODY()

    FHexTile() {};

    FHexTile(FCubeCoord InCoord, ETileType InType)
        : Coord(InCoord),
        TileType(InType)
        {
        
        }
    
    friend bool operator == (const FHexTile& Lhs, const FCubeCoord& Rhs)
    {
        return (Lhs.Coord.q == Rhs.q && Lhs.Coord.r == Rhs.r && Lhs.Coord.s == Rhs.s);
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCubeCoord Coord;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETileType TileType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Cost = 1;
};
