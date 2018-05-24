#pragma once

#include "Vec3.h"

class Ray
{
public:
    constexpr Ray() noexcept;
    constexpr Ray( const FVec3& origin, const FVec3& direction ) noexcept;

    constexpr FVec3 PointAt( F32 t ) const noexcept;

public:
    const FVec3 Origin;
    const FVec3 Direction;
};

constexpr Ray::Ray() noexcept = default;

constexpr Ray::Ray( const FVec3& origin, const FVec3& direction ) noexcept
    : Origin(origin), Direction(direction.Normalize())
{
}

constexpr FVec3 Ray::PointAt(F32 t) const noexcept
{
    return Origin + t * Direction;
}
