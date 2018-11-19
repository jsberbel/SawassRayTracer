// ======================================================================
// File: material.h
// Revision: 1.0
// Creation: 05/24/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v3.h"
#include "core/utils.h"

class Ray;
struct Hit;

class Material
{
MOVABLE_ONLY( Material );

public:
    Material() = default;
    virtual ~Material() noexcept = default;

    virtual b32 scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept = 0;

    static constexpr fv3 reflect(const fv3& _incident, const fv3& _normal) noexcept;
    static constexpr b32 refract(const fv3& _incident, const fv3& _normal, f32 _refractive_ratio, fv3* _refracted) noexcept;
    static constexpr f32 schlick(f32 _cosine, f32 _refractive_idx) noexcept;
};

constexpr fv3 Material::reflect(const fv3& _incident, const fv3& _normal) noexcept
{
    return _incident - 2.f * math::dot(_incident, _normal) * _normal;
}

constexpr b32 Material::refract(const fv3& _incident, const fv3& _normal, f32 _refractive_ratio, fv3* _refracted) noexcept
{
    sws_assert( _refracted );

    const fv3 unit_incident = _incident.get_normalized();
    const f32 cos_incident = math::dot(unit_incident, _normal);
    const f32 d = 1.f - _refractive_ratio * _refractive_ratio * (1.f - cos_incident * cos_incident);
    if (d > 0.f)
    {
        *_refracted = _refractive_ratio * (unit_incident - _normal * cos_incident) - _normal * std::sqrt(d);
        return true;
    }
    return false;
}

constexpr f32 Material::schlick(f32 _cosine, f32 _refractive_idx) noexcept
{
    const f32 r0 = math::sqr((1.f - _refractive_idx) / (1.f + _refractive_idx));
    return r0 + (1.f - r0) * math::pow(1.f - _cosine, 5);
}
