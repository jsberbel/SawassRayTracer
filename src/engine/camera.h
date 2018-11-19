// ======================================================================
// File: camera.h
// Revision: 1.0
// Creation: 05/23/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <engine/ray.h>

class Camera
{
MOVABLE_ONLY( Camera );

public:
    inline Camera(const fv3& lookFrom, const fv3& lookAt, u32 imageWidth, u32 imageHeight, f32 vFOV, f32 aperture, f32 startTime, f32 endTime);

    constexpr Ray trace_ray(f32 s, f32 t) const;

private:
    fv3 m_origin;
    fv3 m_lower_left_corner;
    fv3 m_horizontal;
    fv3 m_vertical;
    fv3 m_forward;
    fv3 m_right;
    fv3 m_up;
    f32 m_lens_radius;
    f32 m_start_time;
    f32 m_end_time;
};

inline Camera::Camera(const fv3& lookFrom, const fv3& lookAt, u32 imageWidth, u32 imageHeight, f32 vFOV, f32 aperture, f32 startTime, f32 endTime)
{
    m_origin = lookFrom;
    m_lens_radius = aperture / 2.f;
    m_start_time = startTime;
    m_end_time = endTime;

    const f32 theta = math::to_radians(vFOV); // vertical FOV is top to bottom in degrees
    const f32 halfHeight = math::tan(theta / 2.f); // tan(theta/2) = ((planeHeight/2)*pixelHeight)/zNear (with zNear = 1 and pixelHeight = 1)
    const f32 aspectRatio = static_cast<f32>(imageWidth) / static_cast<f32>(imageHeight);
    const f32 halfWidth = aspectRatio * halfHeight;
    const fv3 worldUp = fv3::up();

    m_forward = (lookFrom - lookAt).get_normalized();
    m_right = math::cross(worldUp, m_forward).get_normalized();
    m_up = math::cross(m_forward, m_right);

    const f32 focalDistance = (lookFrom - lookAt).get_length();
    const fv3 focalRight = halfWidth * focalDistance * m_right;
    const fv3 focalUp = halfHeight * focalDistance * m_up;
    const fv3 focalForward = focalDistance * m_forward;

    m_lower_left_corner = m_origin - focalRight - focalUp - focalForward;
    m_horizontal = 2.f * focalRight;
    m_vertical = 2.f * focalUp;
}

constexpr Ray Camera::trace_ray(f32 s, f32 t) const
{
    const fv3 rnd_in_lens_disk = m_lens_radius * utils::rnd_point_in_unit_disk();
    const fv3 offset = (m_right * rnd_in_lens_disk.x) + (m_up * rnd_in_lens_disk.y);
    const f32 time = m_start_time + utils::rnd_01() * (m_end_time - m_start_time);
    return Ray(m_origin + offset, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset, time);
}
