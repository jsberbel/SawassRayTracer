// ======================================================================
// File: ray.h
// Revision: 1.0
// Creation: 05/21/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <core/math/v3.h>

class Ray
{
MOVABLE_ONLY( Ray );

public:
    constexpr Ray() noexcept;
    constexpr Ray(const fv3& origin, const fv3& direction, f32 time) noexcept;

    constexpr fv3 point_at(f32 distance) const noexcept;

public:
    fv3 origin;
    fv3 direction;
    f32 time;
};

constexpr Ray::Ray() noexcept
    : origin(0.f)
    , direction(0.f)
    , time(0.f)
{
}

constexpr Ray::Ray(const fv3& origin, const fv3& direction, f32 time) noexcept
    : origin(origin)
    , direction(direction.get_normalized())
    , time(time)
{
}

constexpr fv3 Ray::point_at(f32 distance) const noexcept
{
    return origin + distance * direction;
}
