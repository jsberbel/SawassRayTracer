// ======================================================================
// File: dielectric.h
// Revision: 1.0
// Creation: 05/28/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <engine/material/material.h>
#include <engine/geometry/entity.h>
#include <engine/ray.h>

#include <core/math/math.h>
#include <core/utils.h>

class Dielectric : public Material
{
MOVABLE_ONLY( Dielectric );

public:
    constexpr Dielectric(f32 _refractive_idx) noexcept;

    inline b32 scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept override;

public:
    f32 refractive_idx;
};

constexpr Dielectric::Dielectric(f32 _refractive_idx) noexcept
    : refractive_idx(_refractive_idx)
{
}

inline b32 Dielectric::scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept
{
    sws_assert( _attenuation && _scattered );

    *_attenuation = fv3(1.f); // surface absorbs nothing ( test with fv3(1.f, 1.f, 0.f) )

    fv3 outward_normal;
    f32 refractive_ratio;
    f32 cosine;

    if (math::dot(_ray.direction, _hit.normal) > 0)
    {
        outward_normal = -_hit.normal;
        refractive_ratio = refractive_idx;
        //cosine = refractive_idx * math::Dot( inRay.direction, record.normal ) / inRay.direction.Length();
        cosine = math::dot(_ray.direction, _hit.normal) / _ray.direction.get_length();
        cosine = sqrt(1.f - refractive_idx * refractive_idx * (1.f - cosine * cosine));
    }
    else
    {
        outward_normal = _hit.normal;
        refractive_ratio = 1.f / refractive_idx;
        cosine = -math::dot(_ray.direction, _hit.normal) / _ray.direction.get_length();
    }

    fv3 reflected = reflect(_ray.direction, _hit.normal);
    fv3 refracted;

    const bool is_refracted = refract(_ray.direction, outward_normal, refractive_ratio, &refracted);
    const f32 reflection_probability = (is_refracted) ? schlick(cosine, refractive_idx) : 1.f;

    const bool is_reflected = util::rnd_01() < reflection_probability;
    *_scattered = Ray(_hit.point, (is_reflected) ? reflected : refracted);

    return true;
}
