#pragma once

#include <Engine/Ray.h>

class Camera
{
public:
    inline Camera( const FVec3& lookFrom, const FVec3& lookAt, const F32& aspectRatio,
                   const F32& vFOV, const F32& aperture, const F32& focusDistance );

    constexpr Ray TraceRay( F32 s, F32 t );

public:
    FVec3 Origin;
    FVec3 LowerLeftCorner;
    FVec3 Horizontal;
    FVec3 Vertical;
    FVec3 W, U, V;
    F32 LensRadius;
};

inline Camera::Camera( const FVec3& lookFrom, const FVec3& lookAt, const F32& aspectRatio,
                          const F32& vFOV, const F32& aperture, const F32& focusDistance ) // vertical FOV is top to bottom in degrees
    /*: Origin( lookFrom )
    , LensRadius( aperture / 2.f )
    , W( ( lookFrom - lookAt ).Normalize() )
    , U( Math::Cross( FVec3( 0.f, 1.f, 0.f ), W ).Normalize() )
    , V( Math::Cross( W, U ) )
    , HalfHeight( Math::Tan( ( vFOV * ( Math::PI<F32> / 180.f ) ) / 2.f ) )
    , HalfWidth( aspectRatio * HalfHeight )
    , LowerLeftCorner( Origin - (HalfWidth  * focusDistance * U) - (HalfHeight * focusDistance * V) - (focusDistance * W) )
    , Horizontal( 2.f * HalfWidth * focusDistance * U )
    , Vertical( 2.f * HalfHeight * focusDistance * V )*/
{
    Origin     = lookFrom;
    LensRadius = aperture / 2.f;

    const F32 theta       = vFOV * ( Math::PI<F32> / 180.f );
    const F32 halfHeight  = Math::Tan( theta / 2.f );
    const F32 halfWidth   = aspectRatio * halfHeight;
    const FVec3 up        = FVec3( 0.f, 1.f, 0.f );

    W = ( lookFrom - lookAt ).Normalize();
    U = Math::Cross( up, W ).Normalize();
    V = Math::Cross( W, U );

    const FVec3 right = halfWidth  * focusDistance * U;
    const FVec3 top   = halfHeight * focusDistance * V;
    const FVec3 near  = focusDistance * W;

    LowerLeftCorner = Origin - right - top - near;
    Horizontal      = 2.f * right;
    Vertical        = 2.f * top;
}

constexpr Ray Camera::TraceRay( F32 s, F32 t )
{
    const FVec3 rd = LensRadius * Utils::RandomPointInUnitDisk();
    const FVec3 offset = ( U * rd.X ) + ( V * rd.Y );
    return Ray( Origin + offset, LowerLeftCorner + s*Horizontal + t*Vertical - Origin - offset );
}
