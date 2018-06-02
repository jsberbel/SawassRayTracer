#pragma once

#include <Engine/Ray.h>

#include <Scene/Geometry/Hitable.h>
#include <Scene/Material/Material.h>

#include <Core/Math.h>

class SphereBase : public Hitable
{
    MOVABLE_ONLY( SphereBase );

public:
    constexpr SphereBase( Material* material, F32 radius );
    ~SphereBase() override = default;

    inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord* hitRecord ) const override;

protected:
    constexpr bool Solve( F32 root, F32 tMin, F32 tMax, const Ray& ray, HitRecord* hitRecord ) const;
    virtual inline FVec3 GetCenter( F32 time ) const = 0;

private:
    F32 m_Radius;
};

class StaticSphere : public SphereBase
{
    MOVABLE_ONLY( StaticSphere );

public:
    template <class TMaterial>
    constexpr StaticSphere( const FVec3& center, F32 radius, TMaterial&& material );

    ~StaticSphere() override = default;

protected:
    inline FVec3 GetCenter( F32 time ) const override;

private:
    FVec3 m_Position;
};

class DynamicSphere : public SphereBase
{
    MOVABLE_ONLY( DynamicSphere );

public:
    template <class TMaterial>
    constexpr DynamicSphere( const FVec3& startPosition, const FVec3& endPosition, F32 startTime, F32 endTime, F32 radius, TMaterial&& material );

    ~DynamicSphere() override = default;

protected:
    inline FVec3 GetCenter( F32 time ) const override;

public:
    FVec3 m_StartPosition;
    FVec3 m_EndPosition;
    F32   m_StartTime;
    F32   m_EndTime;
};

// SphereBase ---------------------------------------

constexpr SphereBase::SphereBase( Material* material, F32 radius )
    : Hitable( material )
    , m_Radius( radius )
{
}

constexpr bool SphereBase::Solve( F32 root, F32 tMin, F32 tMax, const Ray& ray, HitRecord* hitRecord ) const
{
    Assert( hitRecord );

    if( root > tMin && root < tMax )
    {
        hitRecord->Distance = root;
        hitRecord->Point = ray.PointAt( hitRecord->Distance );
        hitRecord->Normal = ( hitRecord->Point - GetCenter( ray.Time ) ) / m_Radius;
        return true;
    }
    return false;
}

inline bool SphereBase::Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord* hitRecord ) const
{
    Assert( hitRecord );

    // Sphere equations:
    // x*x + y*y + z*z = R*R
    // Dot( p(t)-C, p(t)-C ) = R*R
    // t*t*Dot(B,B) + 2*t*Dot(B,A-C) + Dot(A-C,A-C) - R*R = 0

    const FVec3 oc = ray.Origin - GetCenter( ray.Time );
    const F32 a = Math::Dot( ray.Direction, ray.Direction );
    const F32 b = Math::Dot( oc, ray.Direction );
    const F32 c = Math::Dot( oc, oc ) - m_Radius * m_Radius;
    const F32 discriminant = b * b - a * c;

    if( discriminant > 0 ) // If solution has 2 real roots
    {
        const F32 root1 = ( -b - std::sqrt( discriminant ) ) / a;
        const F32 root2 = ( -b + std::sqrt( discriminant ) ) / a;

        return( Solve( root1, tMin, tMax, ray, hitRecord ) || Solve( root2, tMin, tMax, ray, hitRecord ) );
    }
    return false;
}

// StaticSphere ---------------------------------------

template <class TMaterial> 
constexpr StaticSphere::StaticSphere( const FVec3& position, F32 radius, TMaterial&& material )
    : SphereBase( new TMaterial( std::forward<TMaterial>( material ) ), radius )
    , m_Position( position )
{
}

inline FVec3 StaticSphere::GetCenter( F32 time ) const
{
    return m_Position;
}

// DynamicSphere ---------------------------------------

template <class TMaterial>
constexpr DynamicSphere::DynamicSphere( const FVec3& startPosition, const FVec3& endPosition, F32 startTime, F32 endTime, F32 radius, TMaterial&& material )
    : SphereBase( new TMaterial( std::forward<TMaterial>( material ) ), radius )
    , m_StartPosition( startPosition )
    , m_EndPosition( endPosition )
    , m_StartTime( startTime )
    , m_EndTime( endTime )
{
}

inline FVec3 DynamicSphere::GetCenter( F32 time ) const
{
    return m_StartPosition + ( ( time - m_StartTime ) / ( m_EndTime - m_StartTime ) ) * ( m_EndPosition - m_StartPosition );
}