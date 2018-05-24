#pragma once

#include <Engine/Ray.h>

#include <Scene/Material/Material.h>
#include <Scene/Geometry/Hitable.h>

#include <Core/Math.h>
#include <Core/Utils.h>

class Metal : public IMaterial
{
public:
    constexpr Metal( const FVec3& albedo, F32 fuzziness ) noexcept;

    inline bool Scatter( const Ray& inRay, const HitRecord& record, FVec3& attenuation, Ray& scattered ) const noexcept override;
    inline FVec3 Reflect( const FVec3& v, const FVec3& n ) const noexcept;

public:
    FVec3 Albedo;
    F32 Fuzziness;
};

constexpr Metal::Metal( const FVec3& albedo, F32 fuzziness ) noexcept
    : Albedo( albedo )
    , Fuzziness( fuzziness < 1.f ? fuzziness : 1.f )
{}

inline bool Metal::Scatter(const Ray& inRay, const HitRecord& record, FVec3& attenuation, Ray& scattered) const noexcept
{
    FVec3 reflected = Reflect( inRay.Direction.Normalize(), record.Normal );
    scattered = Ray( record.Point, reflected + Fuzziness * Utils::RandomPointInUnitSphere() );
    attenuation = Albedo;
    return( Math::Dot( scattered.Direction, record.Normal ) > 0.f );
}

inline FVec3 Metal::Reflect( const FVec3& v, const FVec3& n ) const noexcept
{
    return v - 2.f * Math::Dot( v, n ) * n;
}
