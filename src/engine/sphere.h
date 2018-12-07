// ======================================================================
// File: sphere.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/aabb.h"

#include "engine/ray.h"
#include "engine/entity.h"

class Sphere : public Entity
{
    MOVABLE_ONLY(Sphere);

public:
    explicit Sphere(Transform&& _tf, f32 _radius, Material* _mat);
    ~Sphere() override = default;

    inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const override;
    
    inline b32 compute_aabb(f32 _time, AABB* aabb_) const override;
    inline b32 compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const override;

    static constexpr fv2 get_uv(const fv3& _p);

private:
    f32 radius = 0.f;
    f32 sqr_radius = 0.f;
};

inline Sphere::Sphere(Transform&& _tf, f32 _radius, Material* _mat)
    : Entity(std::move(_tf), _mat)
    , radius(_radius)
    , sqr_radius(_radius*_radius)
{
}

inline b32 Sphere::compute_aabb(f32 _time, AABB* aabb_) const
{
    sws_assert(aabb_);
    aabb_->set(transform.get_position(_time) - fv3(radius), transform.get_position(_time) + fv3(radius));
    return true;
}

inline b32 Sphere::compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const
{
    sws_assert(aabb_);
    const AABB t0_box(transform.get_position(_t0) - fv3(radius), transform.get_position(_t0) + fv3(radius));
    const AABB t1_box(transform.get_position(_t1) - fv3(radius), transform.get_position(_t1) + fv3(radius));
    *aabb_ = AABB::get_surrounding_box(t0_box, t1_box);
    return true;
}

inline constexpr fv2 Sphere::get_uv(const fv3 & _p)
{
    // x = cos(phi)*cos(theta)
    // y = sin(phi)*cos(theta)
    // z = sin(theta)

    const f32 phi   = math::atan2(_p.z, _p.x);
    const f32 theta = math::asin(_p.y);
    const fv2 uv = fv2(
        1.f - (phi + math::fPi) / (2.f * math::fPi),
        (theta + math::fPi / 2.f) / math::fPi
    );
    return uv;
}

inline b32 Sphere::hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const
{
    sws_assert(hit_);

    // Sphere equations:
    // x*x + y*y + z*z = R*R
    // Dot( p(t)-C, p(t)-C ) = R*R
    // t*t*Dot(B,B) + 2*t*Dot(B,A-C) + Dot(A-C,A-C) - R*R = 0

    const fv3 center = transform.get_position(_time);
    const fv3 oc = _ray.origin - center;
    const f32 a = math::dot(_ray.direction, _ray.direction);
    const f32 b = math::dot(oc, _ray.direction);
    const f32 c = math::dot(oc, oc) - sqr_radius;
    const f32 d = b*b - a*c;
    if (d > 0.f)
    {
        f32 root = (-b - math::sqrt(d)) / a;
        if (root <= _zmin || root >= _zmax)
        {
            root = (-b + math::sqrt(d)) / a;
            if (root <= _zmin || root >= _zmax)
                return false;
        }
        hit_->distance = root;
        hit_->point    = _ray.point_at(hit_->distance);
        hit_->normal   = (hit_->point - center) / radius;
        hit_->material = material;
        hit_->uv       = get_uv(hit_->normal);
        return true;
    }
    return false;
}