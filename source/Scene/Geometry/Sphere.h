#pragma once

#include <Engine/Ray.h>

#include <Scene/Geometry/Hitable.h>
#include <Scene/Material/Material.h>

#include <Core/Math.h>

class Sphere : public IHitable
{
public:
    constexpr Sphere();
    constexpr Sphere( const FVec3& center, F32 radius, IMaterial* material );
    ~Sphere() override = default;

    inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const override;

private:
    constexpr bool Solve( F32 root, F32 tMin, F32 tMax, const Ray& ray, HitRecord& record ) const;

public:
    FVec3      Center;
    F32        Radius;
};

constexpr Sphere::Sphere()
    : Center( 0.f )
    , Radius( 0.f )
{
    Material = nullptr;
}

constexpr Sphere::Sphere( const FVec3& center, F32 radius, IMaterial* material )
    : Center( center )
    , Radius( radius )
{
    Material = material;
}

constexpr bool Sphere::Solve( F32 root, F32 tMin, F32 tMax, const Ray& ray, HitRecord& record ) const
{
    if( root > tMin && root < tMax )
    {
        record.Distance = root;
        record.Point = ray.PointAt( record.Distance );
        record.Normal = ( record.Point - Center ) / Radius;
        return true;
    }
    return false;
}

inline bool Sphere::Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const
{
    // Sphere equations:
    // x*x + y*y + z*z = R*R
    // Dot( p(t)-C, p(t)-C ) = R*R
    // t*t*Dot(B,B) + 2*t*Dot(B,A-C) + Dot(A-C,A-C) - R*R = 0

    const FVec3 oc = ray.Origin - Center;
    const F32 a = Math::Dot( ray.Direction, ray.Direction );
    const F32 b = Math::Dot( oc, ray.Direction );
    const F32 c = Math::Dot( oc, oc ) - Radius * Radius;
    const F32 discriminant = b * b - a * c;

    if( discriminant > 0 ) // If solution has 2 real roots
    {
        const F32 root1 = ( -b - std::sqrt( discriminant ) ) / a;
        const F32 root2 = ( -b + std::sqrt( discriminant ) ) / a;

        return( Solve( root1, tMin, tMax, ray, record ) || Solve( root2, tMin, tMax, ray, record ) );
    }
    return false;
}