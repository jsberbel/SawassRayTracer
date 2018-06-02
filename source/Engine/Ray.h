#pragma once

#include <Core/Vec3.h>

class Ray
{
public:
    constexpr Ray() noexcept;
    constexpr Ray( const FVec3& origin, const FVec3& direction ) noexcept;

    constexpr Ray( const Ray& other ) noexcept;
    constexpr Ray( Ray&& other ) noexcept;
    constexpr Ray& operator=( const Ray& other ) noexcept;
    constexpr Ray& operator=( Ray&& other ) noexcept;

    constexpr FVec3 PointAt( F32 distance ) const noexcept;

public:
    FVec3 Origin;
    FVec3 Direction;
};

constexpr Ray::Ray() noexcept
    : Origin( 0.f )
    , Direction( 0.f )
{
}

constexpr Ray::Ray( const FVec3& origin, const FVec3& direction ) noexcept
    : Origin( origin )
    , Direction( direction.Normalize() )
{
}

constexpr Ray::Ray( const Ray& other ) noexcept
    : Origin( other.Origin )
    , Direction( other.Direction )
{
}

constexpr Ray::Ray( Ray&& other ) noexcept
    : Origin( std::move(other.Origin) )
    , Direction( std::move(other.Direction) )
{
}

constexpr Ray& Ray::operator=( const Ray& other ) noexcept
{
    if( this != &other )
    {
        Origin = other.Origin;
        Direction = other.Direction;
    }
    return *this;
}

constexpr Ray& Ray::operator=( Ray&& other ) noexcept
{
    if( this != &other )
    {
        Origin = std::move( other.Origin );
        Direction = std::move( other.Direction );
    }
    return *this;
}

constexpr FVec3 Ray::PointAt( F32 distance ) const noexcept
{
    return Origin + distance * Direction;
}
