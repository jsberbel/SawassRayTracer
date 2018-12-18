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
    inline explicit Sphere(Transform&& _tf, f32 _radius, Material* _mat);
    ~Sphere() override = default;

    inline bool hit(const Ray& _ray, f32 _time, f32 _tmin, f32 _tmax, Hit* hit_) const override;
    inline bool compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const override;

    static constexpr V2f get_uv(const fv3& _p);

private:
    f32 m_radius = 0.f;
    f32 m_sqr_radius = 0.f;
};

class XYRectangle : public Entity
{
    MOVABLE_ONLY(XYRectangle);

public:
    inline explicit XYRectangle(Transform&& _tf, f32 _width, f32 _height, Material* _mat);
    ~XYRectangle() override = default;

    inline bool hit(const Ray& _ray, f32 _time, f32 _tmin, f32 _tmax, Hit* hit_) const override;
    inline bool compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const override;

private:
    f32 m_half_width;
    f32 m_half_height;
};

// Sphere //

inline Sphere::Sphere(Transform&& _tf, f32 _radius, Material* _mat)
    : Entity(std::move(_tf), _mat)
    , m_radius(_radius)
    , m_sqr_radius(_radius*_radius)
{
}

inline bool Sphere::compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const
{
    sws_assert(aabb_);
    if (transform.is_static())
    {
        const fv3 center = transform.get_position(_t0);
        aabb_->set(center - fv3(m_radius), center + fv3(m_radius));
    }
    else
    {
        const AABB box_t0(transform.get_position(_t0) - fv3(m_radius), transform.get_position(_t0) + fv3(m_radius));
        const AABB box_t1(transform.get_position(_t1) - fv3(m_radius), transform.get_position(_t1) + fv3(m_radius));
        *aabb_ = AABB::get_surrounding_box(box_t0, box_t1);
    }
    return true;
}

inline constexpr V2f Sphere::get_uv(const fv3 & _p)
{
    // x = cos(phi)*cos(theta)
    // y = sin(phi)*cos(theta)
    // z = sin(theta)

    const f32 phi   = math::atan2(_p.z, _p.x);
    const f32 theta = math::asin(_p.y);
    const V2f uv = V2f(
        1.f - (phi + math::Pif) / (2.f * math::Pif),
        (theta + math::Pif / 2.f) / math::Pif
    );
    return uv;
}

inline bool Sphere::hit(const Ray& _ray, f32 _time, f32 _tmin, f32 _tmax, Hit* hit_) const
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
    const f32 c = math::dot(oc, oc) - m_sqr_radius;
    const f32 d = b*b - a*c;
    if (d > 0.f)
    {
        f32 root = (-b - math::sqrt(d)) / a;
        if (root <= _tmin || root >= _tmax)
        {
            root = (-b + math::sqrt(d)) / a;
            if (root <= _tmin || root >= _tmax)
                return false;
        }
        hit_->distance = root;
        hit_->point    = _ray.point_at(hit_->distance);
        hit_->normal   = (hit_->point - center) / m_radius;
        hit_->material = material;
        hit_->uv       = get_uv(hit_->normal);
        return true;
    }
    return false;
}

// XYRectangle //

inline XYRectangle::XYRectangle(Transform&& _tf, f32 _width, f32 _height, Material* _mat)
    : Entity(std::move(_tf), _mat)
    , m_half_width(_width * 0.5f)
    , m_half_height(_height * 0.5f)
{
}

inline bool XYRectangle::hit(const Ray& _ray, f32 _time, f32 _tmin, f32 _tmax, Hit* hit_) const
{
    const fv3 center = transform.get_position(_time);
    const f32 t = (center.z - _ray.origin.z) / _ray.direction.z;
    if (t < _tmin || t > _tmax)
        return false;
    
    const f32 x0 = center.x - m_half_width;
    const f32 x1 = center.x + m_half_width;
    const f32 y0 = center.y - m_half_height;
    const f32 y1 = center.y + m_half_height;

    const f32 x = _ray.origin.x + t * _ray.direction.x;
    const f32 y = _ray.origin.y + t * _ray.direction.y;
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    hit_->uv       = V2f((x-x0)/(x1-x0), (y-y0)/(y1-y0));
    hit_->distance = t;
    hit_->material = material;
    hit_->point    = _ray.point_at(t);
    hit_->normal   = fv3::z_axis();
    return true;
}

inline bool XYRectangle::compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const
{
    sws_assert(aabb_);
    if (transform.is_static())
    {
        const fv3 center = transform.get_position(_t0);
        const fv3 offset = fv3(m_half_width, m_half_height, 0.0001f);
        aabb_->set(center - offset, center + offset);
    }
    else
    {
        const fv3 center_t0 = transform.get_position(_t0);
        const fv3 center_t1 = transform.get_position(_t1);
        const fv3 offset = fv3(m_half_width, m_half_height, 0.0001f);
        const AABB box_t0(center_t0 - offset, center_t0 + offset);
        const AABB box_t1(center_t1 - offset, center_t1 + offset);
        *aabb_ = AABB::get_surrounding_box(box_t0, box_t1);
    }
    return true;
}
