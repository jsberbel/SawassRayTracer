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
    inline Camera(const fv3& _look_from, const fv3& _look_at, u32 _img_width, u32 _img_height, f32 _v_FOV, f32 _aperture);

    inline Ray trace_ray(f32 s, f32 t) const;

private:
    fv3 m_origin;
    fv3 m_lower_left_corner;
    fv3 m_horizontal;
    fv3 m_vertical;
    fv3 m_forward;
    fv3 m_right;
    fv3 m_up;
    f32 m_lens_radius;
};

inline Camera::Camera(const fv3& _look_from, const fv3& _look_at, u32 _img_width, u32 _img_height, f32 _v_FOV, f32 _aperture)
{
    m_origin = _look_from;
    m_lens_radius = _aperture / 2.f;

    const f32 theta = math::to_radians(_v_FOV); // vertical FOV is top to bottom in degrees
    const f32 halfHeight = math::tan(theta / 2.f); // tan(theta/2) = ((planeHeight/2)*pixelHeight)/zNear (with zNear = 1 and pixelHeight = 1)
    const f32 aspectRatio = static_cast<f32>(_img_width) / static_cast<f32>(_img_height);
    const f32 halfWidth = aspectRatio * halfHeight;
    const fv3 worldUp = fv3::up();

    m_forward = (_look_from - _look_at).get_normalized();
    m_right = math::cross(worldUp, m_forward).get_normalized();
    m_up = math::cross(m_forward, m_right);

    const f32 focalDistance = (_look_from - _look_at).get_length();
    const fv3 focalRight = halfWidth * focalDistance * m_right;
    const fv3 focalUp = halfHeight * focalDistance * m_up;
    const fv3 focalForward = focalDistance * m_forward;

    m_lower_left_corner = m_origin - focalRight - focalUp - focalForward;
    m_horizontal = 2.f * focalRight;
    m_vertical = 2.f * focalUp;
}

inline Ray Camera::trace_ray(f32 s, f32 t) const
{
    const fv3 rnd_in_lens_disk = m_lens_radius * util::rnd_point_in_unit_disk();
    const fv3 offset = (m_right * rnd_in_lens_disk.x) + (m_up * rnd_in_lens_disk.y);
    return Ray(m_origin + offset, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset);
}
