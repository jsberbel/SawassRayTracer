// ======================================================================
// File: metal.h
// Revision: 1.0
// Creation: 05/25/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <core/math/math.h>
#include <core/utils.h>

#include <engine/ray.h>
#include <engine/material/material.h>
#include <engine/geometry/entity.h>

class Metal : public Material
{
MOVABLE_ONLY( Metal );

public:
    constexpr Metal(const fv3& _albedo, f32 _fuzziness) noexcept;

    inline b32 scatter(const Ray& _ray, const Hit& Hit, fv3* attenuation, Ray* scattered) const noexcept override;

public:
    fv3 albedo;
    f32 fuzziness;
};

constexpr Metal::Metal(const fv3& albedo, f32 fuzziness) noexcept
    : albedo(albedo)
    , fuzziness(math::clamp(fuzziness, 0.f, 1.f))
{
}

inline b32 Metal::scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept
{
    sws_assert( _attenuation && _scattered );

    const fv3 reflected = reflect(_ray.direction.get_normalized(), _hit.normal);
    *_scattered = Ray(_hit.point, reflected + fuzziness * util::rnd_point_in_unit_sphere());
    *_attenuation = albedo;
    return (math::dot(_scattered->direction, _hit.normal) > 0.f);
}
