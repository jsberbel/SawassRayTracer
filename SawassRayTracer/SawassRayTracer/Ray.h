#pragma once

#include "Vec3.h"

class Ray
{
public:
    constexpr Ray() noexcept;
    constexpr Ray( const FVec3& origin, const FVec3& direction ) noexcept;

    constexpr const FVec3& GetOrigin() const;
    constexpr const FVec3& GetDirection() const;
    inline FVec3 GetPointAt( F32 t ) const noexcept;

public:
    const FVec3 m_Origin;
    const FVec3 m_Direction;
};

constexpr Ray::Ray() noexcept = default;

constexpr Ray::Ray( const FVec3& origin, const FVec3& direction ) noexcept
    : m_Origin(origin), m_Direction(direction)
{
}

constexpr const FVec3& Ray::GetOrigin() const
{
    return m_Origin;
}

constexpr const FVec3& Ray::GetDirection() const
{
    return m_Direction;
}

inline FVec3 Ray::GetPointAt(F32 t) const noexcept
{
    return m_Origin + t * m_Direction;
}
