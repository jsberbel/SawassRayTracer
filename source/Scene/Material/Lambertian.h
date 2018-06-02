#pragma once

#include <Engine/Ray.h>

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Hitable.h>

#include <Core/Math.h>
#include <Core/Utils.h>

class Lambertian : public Material
{
    MOVABLE_ONLY( Lambertian );

public:
    constexpr Lambertian( const FVec3& albedo ) noexcept;

    inline bool Scatter( const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered ) const noexcept override;

public:
    FVec3 Albedo;
};

constexpr Lambertian::Lambertian( const FVec3& albedo ) noexcept : Albedo(albedo) {}

inline bool Lambertian::Scatter(const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered) const noexcept
{
    Assert( attenuation && scattered );

    FVec3 target = hitRecord.Point + hitRecord.Normal + Utils::RandomPointInUnitSphere();
    *scattered = Ray( hitRecord.Point, target - hitRecord.Point, ray.Time );
    *attenuation = Albedo; // TODO(jserrano): add scattering probability
    return true;
}