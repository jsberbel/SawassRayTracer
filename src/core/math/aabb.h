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

class AABB
{
    //MOVABLE_ONLY( AABB );

public:
    constexpr AABB() = default;
    constexpr AABB(const fv3& _min, const fv3& _max);

    constexpr void set(const fv3& _min, const fv3& _max);

    constexpr b32 is_hit(const Ray& _ray, f32 _tmin, f32 _tmax) const;

    static constexpr AABB get_surrounding_box(const AABB& _boxA, const AABB& _boxB);

public:
    fv3 min = {};
    fv3 max = {};
};

constexpr AABB::AABB(const fv3& _min, const fv3& _max)
    : min(_min)
    , max(_max)
{
}

constexpr void AABB::set(const fv3& _min, const fv3& _max)
{
    min = _min;
    max = _max;
}

constexpr b32 AABB::is_hit(const Ray& _ray, f32 _tmin, f32 _tmax) const
{
    for (usize i = 0u; i < 3u; ++i)
    {
        const f32 inv_dir = math::inv(_ray.direction[i]);
        const f32 t0 = math::min((min[i] - _ray.origin[i]) * inv_dir,
                                 (max[i] - _ray.origin[i]) * inv_dir);
        const f32 t1 = math::max((min[i] - _ray.origin[i]) * inv_dir,
                                 (max[i] - _ray.origin[i]) * inv_dir);

        _tmin = math::max(t0, _tmin);
        _tmax = math::min(t1, _tmax);
        if (_tmax <= _tmin)
            return false;
    }
    return true;
}

constexpr AABB AABB::get_surrounding_box(const AABB& _boxA, const AABB& _boxB)
{
    const fv3 min(math::min(_boxA.min.x, _boxB.min.x),
                  math::min(_boxA.min.y, _boxB.min.y),
                  math::min(_boxA.min.z, _boxB.min.z));

    const fv3 max(math::max(_boxA.max.x, _boxB.max.x),
                  math::max(_boxA.max.y, _boxB.max.y),
                  math::max(_boxA.max.z, _boxB.max.z));

    return AABB(min, max);
}
