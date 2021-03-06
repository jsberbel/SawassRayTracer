// ======================================================================
// File: material.h
// Revision: 1.0
// Creation: 05/24/2018 - jsberbel
// Notice: Copyright � 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v3.h"
#include "core/utils.h"
#include "engine/texture.h"

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

class Metal : public Material
{
    MOVABLE_ONLY(Metal);

public:
    constexpr Metal(const fv3& _albedo, f32 _fuzziness) noexcept;

    inline b32 scatter(const Ray& _ray, const Hit& Hit, fv3* attenuation, Ray* scattered) const noexcept override;

public:
    fv3 albedo;
    f32 fuzziness;
};

class Dielectric : public Material
{
    MOVABLE_ONLY(Dielectric);

public:
    constexpr Dielectric(f32 _refractive_idx) noexcept;

    inline b32 scatter(const Ray& _ray, const Hit& _hit, fv3* attenuation_, Ray* scattered_) const noexcept override;

public:
    f32 refractive_idx;
};

// -- //

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
    return r0 + (1.f - r0) * math::pow(1.f - _cosine, 5.f);
}

// Lambertian //

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
    *attenuation_ = albedo->value(_hit.uv, _hit.point); // TODO(jserrano): add scattering probability
    *scattered_ = Ray(_hit.point, target - _hit.point);
    return true;
}

// Metal //

constexpr Metal::Metal(const fv3& albedo, f32 fuzziness) noexcept
    : albedo(albedo)
    , fuzziness(math::clamp(fuzziness, 0.f, 1.f))
{
}

inline b32 Metal::scatter(const Ray& _ray, const Hit& _hit, fv3* _attenuation, Ray* _scattered) const noexcept
{
    sws_assert( _attenuation && _scattered );

    const fv3 reflected = reflect(_ray.direction.get_normalized(), _hit.normal);
    *_scattered = Ray(_hit.point, reflected + fuzziness * util::rand_point_in_unit_sphere());
    *_attenuation = albedo;
    return (math::dot(_scattered->direction, _hit.normal) > 0.f);
}

// Dielectric //

constexpr Dielectric::Dielectric(f32 _refractive_idx) noexcept
    : refractive_idx(_refractive_idx)
{
}

inline b32 Dielectric::scatter(const Ray& _ray, const Hit& _hit, fv3* attenuation_, Ray* scattered_) const noexcept
{
    sws_assert(attenuation_ && scattered_);

    *attenuation_ = fv3(1.f); // surface absorbs nothing ( test with fv3(1.f, 1.f, 0.f) )

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

    const bool is_reflected = util::frand_01() < reflection_probability;
    *scattered_ = Ray(_hit.point, (is_reflected) ? reflected : refracted);

    return true;
}
