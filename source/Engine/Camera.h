#pragma once

#include <Engine/Ray.h>

class Camera
{
    MOVABLE_ONLY( Camera );

public:
    inline Camera( const FVec3& lookFrom, const FVec3& lookAt, U32 imageWidth, U32 imageHeight, F32 vFOV, F32 aperture, F32 startTime, F32 endTime );

    constexpr Ray TraceRay( F32 s, F32 t ) const;

private:
    FVec3 m_Origin;
    FVec3 LowerLeftCorner;
    FVec3 Horizontal;
    FVec3 Vertical;
    FVec3 m_Forward;
    FVec3 m_Right;
    FVec3 m_Up;
    F32 m_LensRadius;
    F32 m_StartTime;
    F32 m_EndTime;
};

inline Camera::Camera( const FVec3& lookFrom, const FVec3& lookAt, U32 imageWidth, U32 imageHeight, F32 vFOV, F32 aperture, F32 startTime, F32 endTime )
{
    m_Origin     = lookFrom;
    m_LensRadius = aperture / 2.f;
    m_StartTime  = startTime;
    m_EndTime    = endTime;

    const F32 theta       = Math::ToRadians( vFOV );  // vertical FOV is top to bottom in degrees
    const F32 halfHeight  = Math::Tan( theta / 2.f ); // tan(theta/2) = ((planeHeight/2)*pixelHeight)/zNear (with zNear = 1 and pixelHeight = 1)
    const F32 aspectRatio = static_cast<F32>( imageWidth ) / static_cast<F32>( imageHeight );
    const F32 halfWidth   = aspectRatio * halfHeight;
    const FVec3 worldUp   = FVec3( 0.f, 1.f, 0.f );

    m_Forward = ( lookFrom - lookAt ).Normalize();
    m_Right   = Math::Cross( worldUp, m_Forward ).Normalize();
    m_Up      = Math::Cross( m_Forward, m_Right );

    const F32 focalDistance  = ( lookFrom - lookAt ).Length();
    const FVec3 focalRight   = halfWidth  * focalDistance * m_Right;
    const FVec3 focalUp      = halfHeight * focalDistance * m_Up;
    const FVec3 focalForward = focalDistance * m_Forward;

    LowerLeftCorner = m_Origin - focalRight - focalUp - focalForward;
    Horizontal      = 2.f * focalRight;
    Vertical        = 2.f * focalUp;
}

constexpr Ray Camera::TraceRay( F32 s, F32 t ) const
{
    const FVec3 randomInLensDisk = m_LensRadius * Utils::RandomPointInUnitDisk();
    const FVec3 offset           = ( m_Right * randomInLensDisk.X ) + ( m_Up * randomInLensDisk.Y );
    const F32   time             = m_StartTime + Utils::Random01() * ( m_EndTime - m_StartTime );
    return Ray( m_Origin + offset, LowerLeftCorner + s*Horizontal + t*Vertical - m_Origin - offset, time );
}