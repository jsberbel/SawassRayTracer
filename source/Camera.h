#pragma once

#include "Ray.h"

class Camera
{
public:
    constexpr Camera();
    constexpr Camera( const FVec3& origin, F32 width, F32 height );

    constexpr Ray TraceRay( F32 u, F32 v );

public:
    FVec3 Origin;
    FVec3 LowerLeftCorner;
    FVec3 Horizontal;
    FVec3 Vertical;
};

constexpr Camera::Camera() = default;

constexpr Camera::Camera( const FVec3& origin, F32 width, F32 height )
    : Origin(origin)
{
    const F32 widthRatio = width / 50.f;
    const F32 heightRatio = height / 50.f;
    LowerLeftCorner = FVec3( -widthRatio*0.5f, -heightRatio*0.5f, -1.f );
    Horizontal = FVec3( widthRatio, 0.f, 0.f );
    Vertical = FVec3( 0.f, heightRatio, 0.f );
}

constexpr Ray Camera::TraceRay( F32 u, F32 v )
{
    return Ray( Origin, LowerLeftCorner + u*Horizontal + v*Vertical - Origin );
}
