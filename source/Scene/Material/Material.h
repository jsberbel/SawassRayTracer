#pragma once

#include <Core/Vec3.h>
#include "Core/Math.h"

class Ray;
struct HitRecord;

class IMaterial
{
public:
    virtual ~IMaterial() noexcept = default;
    virtual bool Scatter( const Ray& inRay, const HitRecord& record, FVec3& attenuation, Ray& scattered ) const noexcept = 0;

    static constexpr FVec3 Reflect( const FVec3& vector, const FVec3& normal ) noexcept;
    static constexpr bool Refract( const FVec3& vector, const FVec3& normal, F32 refractiveRatio, FVec3& refracted ) noexcept;
    static constexpr F32 Schlick( F32 cosine, F32 refractiveIndex ) noexcept;
};

constexpr FVec3 IMaterial::Reflect( const FVec3& vector, const FVec3& normal ) noexcept
{
    return vector - 2.f * Math::Dot( vector, normal ) * normal;
}

constexpr bool IMaterial::Refract( const FVec3& vector, const FVec3& normal, F32 refractiveRatio, FVec3& refracted ) noexcept
{
    FVec3 incident = vector.Normalize();
    F32 cosIncident = Math::Dot( incident, normal );
    F32 discriminant = 1.f - refractiveRatio * refractiveRatio * ( 1.f - cosIncident * cosIncident );
    if (discriminant > 0.f)
    {
        refracted = refractiveRatio * ( incident - normal * cosIncident ) - normal * std::sqrt( discriminant );
        return true;
    }
    return false;
}

constexpr F32 IMaterial::Schlick( F32 cosine, F32 refractiveIndex ) noexcept
{
    F32 r0 = ( 1.f - refractiveIndex ) / ( 1.f + refractiveIndex );
    r0 = r0 * r0;
    return r0 + ( 1.f - r0 ) * Math::Pow( 1.f - cosine, 5 );
}
