// ======================================================================
// File: hitable.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <optional>

#include "core/math/v3.h"
#include "engine/material/material.h"

class Ray;
class aabb;

struct Hit
{
MOVABLE_ONLY( Hit );

    fv3 point;
    fv3 normal;
    f32 distance;
    Material* material;

    Hit() = default;
};

class IHitable
{
public:
    virtual ~IHitable() = 0;

    virtual inline bool hit(const Ray& _ray, f32 _tmin, f32 _tmax, Hit* _hit) const = 0;
    virtual inline bool compute_bb(f32 _t0, f32 _t1, aabb* _box) const = 0;
};
