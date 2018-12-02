// ======================================================================
// File: entity.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "engine/transform.h"
#include "engine/hitable.h"

class Entity : public Hitable
{
public:
    constexpr explicit Entity(Transform&& _tf, Material* _mt);
    constexpr Entity(const Entity&) noexcept = delete;
    constexpr Entity& operator=(const Entity&) noexcept = delete;
    inline Entity(Entity&& _other) noexcept;
    constexpr Entity& operator=(Entity&& _other) noexcept;
    virtual inline ~Entity();

    inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const override = 0;

    inline b32 compute_aabb(f32 _time, AABB* aabb_) const override = 0;
    inline b32 compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const override = 0;

public:
    Transform transform;
    Material* material = nullptr;
};

constexpr Entity::Entity(Transform&& _tf, Material* _mt)
    : transform(std::move(_tf))
    , material(_mt)
{}

inline Entity::Entity(Entity&& _other) noexcept
    : transform(std::move(_other.transform))
    , material(std::exchange(_other.material, nullptr))
{
}

inline constexpr Entity & Entity::operator=(Entity&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        transform = std::move(_other.transform);
        material = std::exchange(_other.material, nullptr);
    }
    return *this;
}

inline Entity::~Entity()
{
    util::safe_del(material);
}
