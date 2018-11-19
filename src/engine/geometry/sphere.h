// ======================================================================
// File: sphere.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/aabb.h"

#include "engine/ray.h"
#include "engine/geometry/hit.h"
#include "engine/material/material.h"

class SphereBase : public IHitable
{
MOVABLE_ONLY( SphereBase );

public:
    constexpr SphereBase(Material* _mat, f32 _radius);
    ~SphereBase() override = default;

    inline bool hit(const Ray& _ray, f32 _tmin, f32 _tmax, Hit* _hit) const override;

protected:
    constexpr bool solve(f32 _root, f32 _tmin, f32 _tmax, const Ray& _ray, Hit* _hit) const;
    virtual inline fv3 get_center(f32 _time) const = 0;

public:
    Material* material;

protected:
    f32 m_radius;
};

class StaticSphere : public SphereBase
{
MOVABLE_ONLY( StaticSphere );

public:
    template <class TMaterial>
    constexpr StaticSphere(const fv3& _center, f32 _radius, TMaterial&& _mat);

    ~StaticSphere() override = default;

    inline bool compute_bb(f32 _t0, f32 _t1, aabb* _box) const override;

protected:
    inline fv3 get_center(f32 _time) const override;

private:
    fv3 m_position;
};

class DynamicSphere : public SphereBase
{
MOVABLE_ONLY( DynamicSphere );

public:
    template <class TMaterial>
    constexpr DynamicSphere(const fv3& _start_pos, const fv3& _end_pos, f32 _start_time, f32 _end_time, f32 _radius, TMaterial&& _mat);

    ~DynamicSphere() override = default;

    inline bool compute_bb(f32 _t0, f32 _t1, aabb* _box) const override;

protected:
    inline fv3 get_center(f32 _time) const override;

public:
    fv3 start_pos;
    fv3 end_pos;
    f32 start_time;
    f32 end_time;
};

// SphereBase ---------------------------------------

constexpr SphereBase::SphereBase(Material* _mat, f32 _radius)
    : material(_mat)
    , m_radius(_radius)
{
}

constexpr bool SphereBase::solve(f32 _root, f32 _tmin, f32 _tmax, const Ray& _ray, Hit* _hit) const
{
    sws_assert( _hit );

    if (_root > _tmin && _root < _tmax)
    {
        _hit->distance = _root;
        _hit->point    = _ray.point_at(_hit->distance);
        _hit->normal   = (_hit->point - get_center(_ray.time)) / m_radius;
        return true;
    }
    return false;
}

inline bool SphereBase::hit(const Ray& _ray, f32 _tmin, f32 _tmax, Hit* _hit) const
{
    sws_assert( _hit );

    // Sphere equations:
    // x*x + y*y + z*z = R*R
    // Dot( p(t)-C, p(t)-C ) = R*R
    // t*t*Dot(B,B) + 2*t*Dot(B,A-C) + Dot(A-C,A-C) - R*R = 0

    const fv3 oc = _ray.origin - get_center(_ray.time);
    const f32 a = math::dot(_ray.direction, _ray.direction);
    const f32 b = math::dot(oc, _ray.direction);
    const f32 c = math::dot(oc, oc) - m_radius * m_radius;
    const f32 d = b * b - a * c;

    if (d > 0) // if solution has 2 real roots
    {
        const f32 root1 = (-b - std::sqrt(d)) / a;
        const f32 root2 = (-b + std::sqrt(d)) / a;

        return solve(root1, _tmin, _tmax, _ray, _hit) || 
               solve(root2, _tmin, _tmax, _ray, _hit);
    }
    return false;
}

// StaticSphere ---------------------------------------

template <class TMaterial>
constexpr StaticSphere::StaticSphere(const fv3& _center, f32 _radius, TMaterial&& _material)
    : SphereBase(new TMaterial(std::forward<TMaterial>(_mat)), _radius)
    , m_position(_center)
{
}

bool StaticSphere::compute_bb(f32 _t0, f32 _t1, aabb* _box) const
{
    sws_assert( _box );
    *_box = aabb(m_position - fv3(m_radius), m_position + fv3(m_radius));
    return true;
}

inline fv3 StaticSphere::get_center(f32 _time) const
{
    return m_position;
}

// DynamicSphere ---------------------------------------

template <class TMaterial>
constexpr DynamicSphere::DynamicSphere(const fv3& _start_pos, const fv3& _end_pos, f32 _start_time, f32 _end_time, f32 _radius, TMaterial&& _mat)
    : SphereBase(new TMaterial(std::forward<TMaterial>(_mat)), _radius)
    , m_start_pos(_start_pos)
    , m_end_pos(_end_pos)
    , m_start_time(_start_time)
    , m_end_time(_end_time)
{
}

bool DynamicSphere::compute_bb(f32 _t0, f32 _t1, aabb* _box) const
{
    sws_assert( _box );
    aabb t0_box(get_center(_t0) - fv3(m_radius), get_center(_t0) + fv3(m_radius));
    aabb t1_box(get_center(_t1) - fv3(m_radius), get_center(_t1) + fv3(m_radius));
    *_box = aabb::get_surrounding_box(t0_box, t1_box);
    return true;
}

inline fv3 DynamicSphere::get_center(f32 _time) const
{
    return start_pos + ((_time - start_time) / (end_time - start_time)) * (end_pos - start_pos);
}
