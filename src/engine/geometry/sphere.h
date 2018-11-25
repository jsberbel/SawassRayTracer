// ======================================================================
// File: sphere.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/aabb.h"

#include "engine/ray.h"
#include "engine/geometry/entity.h"

class Sphere : public Entity
{
    //MOVABLE_ONLY(Sphere);

public:
    template <class TM, ENABLE_IF(IS_BASE_OF(TM, Material))>
    explicit Sphere(const fv3& _center, f32 _radius, TM&& _mat);
    template <class TM, ENABLE_IF(IS_BASE_OF(TM, Material))>
    explicit Sphere(const fv3& _start_pos, const fv3& _target_pos, f32 _radius, TM&& _mat);
    ~Sphere() override = default;

    inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const override;

    inline AABB get_bounding_box(f32 _time) const override;
    inline AABB get_bounding_box(f32 _t0, f32 _t1) const override;

protected:
    constexpr b32 solve(f32 _root, f32 _time, f32 _zmin, f32 _zmax, const Ray& _ray, Hit* hit_) const;

public:
    f32 radius = 0.f;
};

template <class TM, class>
inline Sphere::Sphere(const fv3& _center, f32 _radius, TM&& _mat)
    : Entity(new TM(std::forward<TM>(_mat)))
    , radius(_radius)
{
    Entity::set_position(_center);
}

template <class TM, class>
inline Sphere::Sphere(const fv3& _start_pos, const fv3& _target_pos, f32 _radius, TM&& _mat)
    : Entity(new TM(std::forward<TM>(_mat)))
    , radius(_radius)
{
    Entity::set_position(_start_pos, _target_pos);
}

inline AABB Sphere::get_bounding_box(f32 _time) const
{
    return AABB(get_position(_time) - fv3(radius), get_position(_time) + fv3(radius));
}

inline AABB Sphere::get_bounding_box(f32 _t0, f32 _t1) const
{
    const AABB t0_box(get_position(_t0) - fv3(radius), get_position(_t0) + fv3(radius));
    const AABB t1_box(get_position(_t1) - fv3(radius), get_position(_t1) + fv3(radius));
    return AABB::get_surrounding_box(t0_box, t1_box);
}

constexpr b32 Sphere::solve(f32 _root, f32 _time, f32 _zmin, f32 _zmax, const Ray& _ray, Hit* hit_) const
{
    sws_assert( hit_ );

    if (_root > _zmin && _root < _zmax)
    {
        hit_->distance = _root;
        hit_->point    = _ray.point_at(hit_->distance);
        hit_->normal   = (hit_->point - get_position(_time)) / radius;
        return true;
    }
    return false;
}

inline b32 Sphere::hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const
{
    sws_assert( hit_ );

    // Sphere equations:
    // x*x + y*y + z*z = R*R
    // Dot( p(t)-C, p(t)-C ) = R*R
    // t*t*Dot(B,B) + 2*t*Dot(B,A-C) + Dot(A-C,A-C) - R*R = 0

    const fv3 oc = _ray.origin - get_position(_time);
    const f32 a = math::dot(_ray.direction, _ray.direction);
    const f32 b = math::dot(oc, _ray.direction);
    const f32 c = math::dot(oc, oc) - radius * radius;
    const f32 d = b * b - a * c;

    if (d > 0) // if solution has 2 real roots
    {
        const f32 root1 = (-b - std::sqrt(d)) / a;
        const f32 root2 = (-b + std::sqrt(d)) / a;

        return solve(root1, _time, _zmin, _zmax, _ray, hit_) || 
               solve(root2, _time, _zmin, _zmax, _ray, hit_);
    }
    return false;
}