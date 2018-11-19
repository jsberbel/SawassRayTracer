// ======================================================================
// File: lambertian.h
// Revision: 1.0
// Creation: 05/24/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <core/utils.h>

#include <engine/ray.h>
#include <engine/material/material.h>
#include <engine/geometry/hit.h>

class Lambertian : public Material
{
MOVABLE_ONLY( Lambertian );

public:
    constexpr Lambertian(const fv3& _albedo) noexcept;

    inline b32 scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept override;

public:
    fv3 albedo;
};

constexpr Lambertian::Lambertian(const fv3& _albedo) noexcept : albedo(_albedo)
{
}

inline b32 Lambertian::scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept
{
    sws_assert( _attenuation && _scattered );

    const fv3 target = _hit.point + _hit.normal + utils::rnd_point_in_unit_sphere();
    *_scattered = Ray(_hit.point, target - _hit.point, _ray.time);
    *_attenuation = albedo; // TODO(jserrano): add scattering probability
    return true;
}
