// ======================================================================
// File: lambertian.h
// Revision: 1.0
// Creation: 05/24/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <core/utils.h>

#include <engine/ray.h>
#include <engine/material.h>
#include <engine/entity.h>
#include "engine/texture.h"

class Lambertian : public Material
{
public:
    constexpr Lambertian(Texture* _albedo) noexcept;
    constexpr Lambertian(const Lambertian&) noexcept = delete;
    constexpr Lambertian& operator=(const Lambertian&) noexcept = delete;
    inline Lambertian(Lambertian&& _other) noexcept;
    constexpr Lambertian& operator=(Lambertian&& _other) noexcept;
    virtual inline ~Lambertian();

    inline b32 scatter(const Ray& _ray, const Hit& _hit, fv3* attenuation_, Ray* scattered_) const noexcept override;

public:
    Texture* albedo;
};

constexpr Lambertian::Lambertian(Texture* _albedo) noexcept
    : albedo(_albedo)
{
}

inline Lambertian::Lambertian(Lambertian&& _other) noexcept
    : albedo(std::exchange(_other.albedo, nullptr))
{
}

inline constexpr Lambertian& Lambertian::operator=(Lambertian&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        albedo = std::exchange(_other.albedo, nullptr);
    }
    return *this;
}

inline Lambertian::~Lambertian()
{
    util::safe_del(albedo);
}

inline b32 Lambertian::scatter(const Ray& _ray, const Hit& _hit, fv3* attenuation_, Ray* scattered_) const noexcept
{
    sws_assert(attenuation_ && scattered_);

    const fv3 target = _hit.point + _hit.normal + util::rand_point_in_unit_sphere();
    *scattered_ = Ray(_hit.point, target - _hit.point);
    *attenuation_ = albedo->value(0, 0, _hit.point); // TODO(jserrano): add scattering probability
    return true;
}
