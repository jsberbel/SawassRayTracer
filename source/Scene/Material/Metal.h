#pragma once

#include <Engine/Ray.h>

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Hitable.h>

#include <Core/Math.h>
#include <Core/Utils.h>

class Metal : public Material
{
    MOVABLE_ONLY( Metal );

public:
    constexpr Metal( const FVec3& albedo, F32 fuzziness ) noexcept;

    inline bool Scatter( const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered ) const noexcept override;

public:
    FVec3 Albedo;
    F32 Fuzziness;
};

constexpr Metal::Metal( const FVec3& albedo, F32 fuzziness ) noexcept
    : Albedo( albedo )
    , Fuzziness( Math::Clamp( fuzziness, 0.f, 1.f ) )
{}

inline bool Metal::Scatter(const Ray& ray, const HitRecord& hitRecord, FVec3* attenuation, Ray* scattered) const noexcept
{
    Assert( attenuation && scattered );

    FVec3 reflected = Reflect( ray.Direction.Normalize(), hitRecord.Normal );
    *scattered = Ray( hitRecord.Point, reflected + Fuzziness * Utils::RandomPointInUnitSphere(), ray.Time );
    *attenuation = Albedo;
    return( Math::Dot( scattered->Direction, hitRecord.Normal ) > 0.f );
}
