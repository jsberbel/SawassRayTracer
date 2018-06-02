#pragma once

#include <optional>

#include "Core/Vec3.h"

class Ray;
class Material;

struct HitRecord
{
    MOVABLE_ONLY( HitRecord );

    F32 Distance;
    FVec3 Point;
    FVec3 Normal;
    Material* Material;

    HitRecord() = default;
};

class Hitable
{
    MOVABLE_ONLY( Hitable );

public:
    constexpr Hitable( Material* material );
    virtual inline ~Hitable();

    virtual inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord* hitRecord ) const = 0;
    constexpr const Material* GetMaterial() const;

public:
    Material* m_Material;
};

constexpr Hitable::Hitable( Material* material )
    : m_Material( material )
{
}

inline Hitable::~Hitable()
{
    Utils::SafeDelete( m_Material );
}

constexpr const Material* Hitable::GetMaterial() const
{
    return m_Material;
}
