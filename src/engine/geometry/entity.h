// ======================================================================
// File: hitable.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v3.h"
#include "core/math/aabb.h"
#include "engine/material/material.h"

#include <map>

class Ray;
class AABB;

struct Hit
{
    MOVABLE_ONLY(Hit);

    fv3 point          = {};
    fv3 normal         = {};
    f32 distance       = 0.f;
    Material* material = nullptr;

    Hit() = default;
};

class Entity
{
public:
    Entity() = default;
    explicit Entity(Material* _mat) : material(_mat) {};
    virtual ~Entity() = default;

    virtual inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const = 0;
    
    virtual inline void set_position(const fv3& _static_pos);
    virtual inline void set_position(const fv3& _start_pos, const fv3& _end_pos);
    virtual inline fv3 get_position(f32 _time) const;

    virtual inline AABB get_bounding_box(f32 _time) const = 0;
    virtual inline AABB get_bounding_box(f32 _t0, f32 _t1) const = 0;

public:
    Material* material = nullptr;

protected:
    fv3 m_start_pos;
    fv3 m_offset_pos;
    b32 m_is_static;
};

inline void Entity::set_position(const fv3& _static_pos)
{
    m_is_static = true;
    m_start_pos = _static_pos;
    m_offset_pos = fv3::zero();
}

inline void Entity::set_position(const fv3& _start_pos, const fv3& _end_pos)
{
    if (_start_pos == _end_pos)
        m_is_static = true;

    m_start_pos = _start_pos;
    m_offset_pos = _end_pos - m_start_pos;
}

inline fv3 Entity::get_position(f32 _time) const
{
    return (m_is_static) ? m_start_pos : m_start_pos + _time * m_offset_pos;
};