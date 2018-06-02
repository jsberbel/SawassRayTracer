#pragma once

#include <optional>

#include "Core/Vec3.h"

class Ray;
class IMaterial;

struct HitRecord
{
    F32 Distance;
    FVec3 Point;
    FVec3 Normal;
    IMaterial* Material;
};

class IHitable
{
public:
    virtual inline ~IHitable();
    virtual inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& hitRecord ) const = 0;

public:
    IMaterial* Material;
};

inline IHitable::~IHitable()
{
    Utils::SafeDelete( Material );
}
