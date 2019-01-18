// ======================================================================
// File: ray.h
// Revision: 1.0
// Creation: 05/21/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <core/math/algebra.h>

class Ray
{
    MOVABLE_ONLY( Ray );

public:
    constexpr Ray() noexcept;
    constexpr Ray(const P3f& origin, const V3f& direction) noexcept;

    constexpr P3f operator()(fx t) const noexcept;

public:
    P3f o;
    V3f d;
    mutable fx t_max;
    fx time;
    const Ambience* ambience;
};

constexpr Ray::Ray() noexcept
    : o(0.f)
    , d(0.f)
{
}

constexpr Ray::Ray(const P3f& origin, const V3f& direction) noexcept
    : o(origin)
    , d(direction.get_normalized())
{
}

constexpr P3f Ray::operator()(fx t) const noexcept
{
    return o + t * d;
}
