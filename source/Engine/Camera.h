#pragma once

#include <Engine/Ray.h>

class Camera
{
public:
    inline Camera( FVec3 lookFrom, FVec3 lookAt, F32 width, F32 height, F32 vFOV );
    constexpr Ray TraceRay( F32 u, F32 v );

public:
    FVec3 Origin;
    FVec3 LowerLeftCorner;
    FVec3 Horizontal;
    FVec3 Vertical;
};

inline Camera::Camera( FVec3 lookFrom, FVec3 lookAt, F32 width, F32 height, F32 vFOV ) // vertical FOV is top to bottom in degrees
{
    const F32 aspectRatio = width / height;
    const F32 theta       = vFOV * ( Math::PI<F32> / 180.f );
    const F32 halfHeight  = std::tan( theta / 2.f );
    const F32 halfWidth   = aspectRatio * halfHeight;
    const FVec3 up        = FVec3( 0.f, 1.f, 0.f );

    const FVec3 w = ( lookFrom - lookAt ).Normalize();
    const FVec3 u = Math::Cross( up, w ).Normalize();
    const FVec3 v = Math::Cross( w, u );

    Origin          = lookFrom;
    LowerLeftCorner = FVec3( -halfWidth, -halfHeight, -1.f );
    LowerLeftCorner = Origin - halfWidth * u - halfHeight * v - w;
    Horizontal      = 2.f * halfWidth  * u;
    Vertical        = 2.f * halfHeight * v;
}

constexpr Ray Camera::TraceRay( F32 u, F32 v )
{
    return Ray( Origin, LowerLeftCorner + u*Horizontal + v*Vertical - Origin );
}
