// ======================================================================
// File: transform.h
// Revision: 1.0
// Creation: 12/01/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/utils.h"
#include "core/math/v3.h"

class Transform
{
    MOVABLE_ONLY(Transform);

public:
    Transform() = delete;
    explicit inline Transform(const fv3& _pos);
    explicit inline Transform(const fv3& _start, const fv3& _end);

    inline bool is_static() const;
    inline fv3 get_position(f32 _time) const;

private:
    fv3 m_start;
    fv3 m_target_offset;
    bool m_is_static;
};

inline Transform::Transform(const fv3& _pos)
{
    m_start = _pos;
    m_is_static = true;
}

inline Transform::Transform(const fv3& _start, const fv3& _end)
{
    m_start = _start;
    m_target_offset = _end - _start;
    m_is_static = (_start == _end);
}

inline bool Transform::is_static() const
{
    return m_is_static;
}

inline fv3 Transform::get_position(f32 _time) const
{
    return (m_is_static) ? m_start : (m_start + _time * m_target_offset);
};
