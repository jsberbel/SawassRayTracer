#pragma once

#include <Engine/Ray.h>

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Hitable.h>

#include <Core/Math.h>
#include <Core/Utils.h>

class Lambertian : public IMaterial
{
public:
    constexpr Lambertian( const FVec3& albedo ) noexcept;

    inline bool Scatter( const Ray& inRay, const HitRecord& record, FVec3& attenuation, Ray& scattered ) const noexcept override;

public:
    FVec3 Albedo;
};

constexpr Lambertian::Lambertian( const FVec3& albedo ) noexcept : Albedo(albedo) {}

inline bool Lambertian::Scatter(const Ray& inRay, const HitRecord& record, FVec3& attenuation, Ray& scattered) const noexcept
{
    FVec3 target = record.Point + record.Normal + Utils::RandomPointInUnitSphere();
    scattered = Ray( record.Point, target - record.Point );
    attenuation = Albedo; // TODO(jserrano): add scattering probability
    //FVec3 target = record.Point + record.Normal + Utils::RandomPointInUnitSphere();
    //return 0.5f * GenerateColor( Ray( record.Point, target - record.Point ), world );
    return true;
}