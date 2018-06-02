#pragma once

#include <Core/Vec3.h>
#include "Core/Math.h"

class Ray;
struct HitRecord;

class Material
{
    MOVABLE_ONLY( Material );

public:
    Material() = default;
    virtual ~Material() noexcept = default;

    virtual bool Scatter( const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered ) const noexcept = 0;

    static constexpr FVec3 Reflect( const FVec3& incident, const FVec3& normal ) noexcept;
    static constexpr bool  Refract( const FVec3& incident, const FVec3& normal, F32 refractiveRatio, FVec3* refracted ) noexcept;
    static constexpr F32   Schlick( F32 cosine, F32 refractiveIndex ) noexcept;
};

constexpr FVec3 Material::Reflect( const FVec3& incident, const FVec3& normal ) noexcept
{
    return incident - 2.f * Math::Dot( incident, normal ) * normal;
}

constexpr bool Material::Refract( const FVec3& incident, const FVec3& normal, F32 refractiveRatio, FVec3* refracted ) noexcept
{
    Assert( refracted );

    FVec3 unitIncident = incident.Normalize();
    F32 cosIncident = Math::Dot( unitIncident, normal );
    F32 discriminant = 1.f - refractiveRatio*refractiveRatio * ( 1.f - cosIncident*cosIncident );
    if (discriminant > 0.f)
    {
        *refracted = refractiveRatio * ( unitIncident - normal*cosIncident ) - normal*std::sqrt( discriminant );
        return true;
    }
    return false;
}

constexpr F32 Material::Schlick( F32 cosine, F32 refractiveIndex ) noexcept
{
    F32 r0 = ( 1.f - refractiveIndex ) / ( 1.f + refractiveIndex );
    r0 = r0 * r0;
    return r0 + ( 1.f - r0 ) * Math::Pow( 1.f - cosine, 5 );
}
