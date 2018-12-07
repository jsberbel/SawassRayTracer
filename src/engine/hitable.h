// ======================================================================
// File: hitable.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v3.h"

class Ray;
class AABB;
class Material;

struct Hit
{
    MOVABLE_ONLY(Hit);

    Hit() = default;

    fv3 point          = {};
    fv3 normal         = {};
    fv2 uv             = {};
    f32 distance       = 0.f;
    Material* material = nullptr;
};

class Hitable
{
public:
    Hitable() = default;
    virtual ~Hitable() = default;

    virtual inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const = 0;

    virtual inline b32 compute_aabb(f32 _time, AABB* aabb_) const = 0;
    virtual inline b32 compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const = 0;
};