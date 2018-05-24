#pragma once

#include <Core/Vec3.h>

class Ray;
struct HitRecord;

class IMaterial
{
public:
    virtual ~IMaterial() noexcept = default;
    virtual bool Scatter( const Ray& inRay, const HitRecord& record, FVec3& attenuation, Ray& scattered ) const noexcept = 0;
};