#pragma once

#include <Core/Vec3.h>

class Ray
{
    MOVABLE_ONLY( Ray );

public:
    constexpr Ray() noexcept;
    constexpr Ray( const FVec3& origin, const FVec3& direction, F32 time ) noexcept;

    constexpr FVec3 PointAt( F32 distance ) const noexcept;

public:
    FVec3 Origin;
    FVec3 Direction;
    F32 Time;
};

constexpr Ray::Ray() noexcept
    : Origin( 0.f )
    , Direction( 0.f )
    , Time( 0.f )
{
}

constexpr Ray::Ray( const FVec3& origin, const FVec3& direction, F32 time ) noexcept
    : Origin( origin )
    , Direction( direction.Normalize() )
    , Time( time )
{
}

constexpr FVec3 Ray::PointAt( F32 distance ) const noexcept
{
    return Origin + distance * Direction;
}
