// ======================================================================
// File: world.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/utils.h"

#include "engine/hitable.h"
#include "engine/camera.h"

class HitableList : public Hitable
{
public:
    constexpr HitableList();
    inline HitableList(u32 _capacity);
    constexpr HitableList(const HitableList&) noexcept = delete;
    constexpr HitableList& operator=(const HitableList&) noexcept = delete;
    inline HitableList(HitableList&& _other) noexcept;
    constexpr HitableList& operator=(HitableList&& _other) noexcept;
    virtual inline ~HitableList();

    template <class T, class... TArgs, ENABLE_IF(IS_BASE_OF(T, Hitable))>
    constexpr void add(TArgs&&... _args);

    constexpr Hitable*& operator[](mem_idx idx) const;
    constexpr Hitable** get_buffer() const;
    constexpr u32 get_size() const;

    inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const override;
    inline b32 compute_aabb(f32 _time, AABB* aabb_) const override;
    inline b32 compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const override;

private:
    Hitable** m_hitables = nullptr;
    u32 m_size = 0u;
    u32 m_capacity = 0u;
};

constexpr HitableList::HitableList() = default;

inline HitableList::HitableList(u32 _capacity)
    : m_hitables(new Hitable*[_capacity])
    , m_size(0u)
    , m_capacity(_capacity)
{
}

inline HitableList::HitableList(HitableList&& _other) noexcept
    : m_hitables(std::exchange(_other.m_hitables, nullptr))
    , m_size(_other.m_size)
    , m_capacity(_other.m_capacity)
{
}

inline constexpr HitableList& HitableList::operator=(HitableList&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        m_hitables = std::exchange(_other.m_hitables, nullptr);
        m_size = _other.m_size;
        m_capacity = _other.m_capacity;
    }
    return *this;
}

inline HitableList::~HitableList()
{
    for (mem_idx idx = 0; idx < m_size; ++idx)
        util::safe_del(m_hitables[idx]);
}

template <class T, class... TArgs, class>
constexpr void HitableList::add(TArgs&&... _args)
{
    sws_assert(m_size+1 < m_capacity); // TODO!
    m_hitables[m_size++] = new T(std::forward<TArgs>(_args)...);
}

inline constexpr Hitable*& HitableList::operator[](mem_idx idx) const
{
    sws_assert(idx < m_size);
    return m_hitables[idx];
}

inline constexpr Hitable** HitableList::get_buffer() const
{
    return m_hitables;
}

inline constexpr u32 HitableList::get_size() const
{
    return m_size;
}

inline b32 HitableList::hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const
{
    sws_assert(hit_);

    Hit tmp_hit;
    b32 has_hit_anything = false;
    f32 closest_dist = _zmax;

    for (mem_idx idx = 0; idx < m_size; ++idx)
    {
        if (m_hitables[idx]->hit(_ray, _time, _zmin, closest_dist, &tmp_hit))
        {
            has_hit_anything = true;
            closest_dist = tmp_hit.distance;
            *hit_ = std::move(tmp_hit);
        }
    }

    return has_hit_anything;
}

inline b32 HitableList::compute_aabb(f32 _time, AABB* aabb_) const
{
    return false;
};

inline b32 HitableList::compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const
{
    if (m_size == 0)
        return false;
    
    AABB tmp_aabb;
    if (!m_hitables[0]->compute_aabb(_t0, _t1, &tmp_aabb))
        return false;

    *aabb_ = tmp_aabb;
    for (mem_idx idx = 1u; idx < m_size; ++idx)
    {
        if (!m_hitables[idx]->compute_aabb(_t0, _t1, &tmp_aabb))
            return false;

        *aabb_ = AABB::get_surrounding_box(*aabb_, tmp_aabb);
    }
    return true;
}