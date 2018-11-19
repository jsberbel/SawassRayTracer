// ======================================================================
// File: aabb.h
// Revision: 1.0
// Creation: 06/16/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v3.h"
#include "core/math/math.h"
#include "core/utils.h"
#include "engine/ray.h"

#include <algorithm>

class aabb
{
MOVABLE_ONLY( aabb );

public:
    constexpr aabb();
    constexpr aabb(const fv3& _min, const fv3& _max);
    constexpr aabb(fv3&& _min, fv3&& _max);

    constexpr b32 hit(const Ray& _ray, f32 _tmin, f32 _tmax) const;

    static constexpr aabb get_surrounding_box(const aabb& _boxA, const aabb& _boxB);

public:
    fv3 min;
    fv3 max;
};

constexpr aabb::aabb()
    : min(0.f)
    , max(0.f)
{
}

constexpr aabb::aabb(const fv3& min, const fv3& max)
    : min(min)
    , max(max)
{
}

constexpr aabb::aabb(fv3&& min, fv3&& max)
    : min(std::move(min))
    , max(std::move(min))
{
}

constexpr b32 aabb::hit(const Ray& _ray, f32 _tmin, f32 _tmax) const
{
    for (mem_idx i = 0; i < 3; ++i)
    {
        const f32 inv_dir = math::inv(_ray.direction[i]);
        f32 t0 = (min[i] - _ray.origin[i]) * inv_dir;
        f32 t1 = (max[i] - _ray.origin[i]) * inv_dir;

        if (inv_dir < 0.f)
            std::swap(t0, t1);

        _tmin = math::max(t0, _tmin);
        _tmax = math::min(t1, _tmax);

        if (_tmin < _tmax)
            return false;
    }
    return true;
}

constexpr aabb aabb::get_surrounding_box(const aabb& _boxA, const aabb& _boxB)
{
    const fv3 small(math::min(_boxA.min.x, _boxB.min.x),
                    math::min(_boxA.min.y, _boxB.min.y),
                    math::min(_boxA.min.z, _boxB.min.z));

    const fv3 big(math::max(_boxA.max.x, _boxB.max.x),
                  math::max(_boxA.max.y, _boxB.max.y),
                  math::max(_boxA.max.z, _boxB.max.z));

    return aabb(small, big);
}
