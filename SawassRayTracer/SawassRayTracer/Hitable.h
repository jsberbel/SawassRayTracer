#pragma once

#include "Ray.h"

struct HitRecord
{
    F32 Distance;
    FVec3 Point;
    FVec3 Normal;
};

class IHitable
{
public:
    virtual bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const = 0;
};