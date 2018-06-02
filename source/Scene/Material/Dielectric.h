#pragma once

#include <Engine/Ray.h>

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Hitable.h>

#include <Core/Math.h>
#include <Core/Utils.h>

class Dielectric : public Material
{
    MOVABLE_ONLY( Dielectric );

public:
    constexpr Dielectric( F32 refractiveIndex ) noexcept;

    inline bool Scatter( const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered ) const noexcept override;

public:
    F32 RefractiveIndex;
};

constexpr Dielectric::Dielectric( F32 refractiveIndex ) noexcept
    : RefractiveIndex( refractiveIndex )
{
}

inline bool Dielectric::Scatter( const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered ) const noexcept
{
    Assert( attenuation && scattered );

    *attenuation = FVec3( 1.f ); // surface absorbs nothing ( test with FVec3(1.f, 1.f, 0.f) )
    
    FVec3 outwardNormal;
    F32 refractiveRatio;
    F32 cosine;

    if( Math::Dot( ray.Direction, hitRecord.Normal ) > 0 )
    {
        outwardNormal = -hitRecord.Normal;
        refractiveRatio = RefractiveIndex;
        //cosine = RefractiveIndex * Math::Dot( inRay.Direction, record.Normal ) / inRay.Direction.Length();
        cosine = Math::Dot( ray.Direction, hitRecord.Normal ) / ray.Direction.Length();
        cosine = sqrt( 1.f - RefractiveIndex * RefractiveIndex * ( 1.f - cosine * cosine ) );
    }
    else
    {
        outwardNormal = hitRecord.Normal;
        refractiveRatio = 1.f / RefractiveIndex;
        cosine = -Math::Dot( ray.Direction, hitRecord.Normal ) / ray.Direction.Length();
    }

    FVec3 reflected = Reflect( ray.Direction, hitRecord.Normal );
    FVec3 refracted;
    F32 reflectionProbability;

    const bool isRefracted = Refract( ray.Direction, outwardNormal, refractiveRatio, &refracted );
    reflectionProbability = ( isRefracted ) ? Schlick( cosine, RefractiveIndex ) : 1.f;

    const bool isReflected = Utils::Random01() < reflectionProbability;
    *scattered = Ray( hitRecord.Point, ( isReflected ) ? reflected : refracted, ray.Time );

    return true;
}