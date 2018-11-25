// ======================================================================
// File: world.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/utils.h"

#include "engine/geometry/entity.h"
#include "engine/camera.h"

#include <vector>

class World
{
MOVABLE_ONLY( World );

public:
    constexpr World();
    inline World(u32 _entity_count);
    inline ~World();

    template <class T, class... TArgs, ENABLE_IF(IS_BASE_OF(T, Entity))>
    constexpr T* add(TArgs&&... _args);

    inline b32 hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const;

private:
    std::vector<Entity*> m_entities;
};

constexpr World::World() = default;

inline World::World(u32 _entity_count)
{
    m_entities.reserve(_entity_count);
}

inline World::~World()
{
    for (auto& entity : m_entities)
        util::safe_del(entity);
    
    m_entities.clear();
}

template <class T, class... TArgs, class>
constexpr T* World::add(TArgs&&... _args)
{
    return static_cast<T*>(m_entities.emplace_back(new T(std::forward<TArgs>(_args)...)));
}

inline b32 World::hit(const Ray& _ray, f32 _time, f32 _zmin, f32 _zmax, Hit* hit_) const
{
    Hit last_hit;
    b32 has_hit_anything = false;
    f32 closest_dist = _zmax;

    for (const auto& entity : m_entities)
    {
        if (entity->hit(_ray, _time, _zmin, closest_dist, &last_hit))
        {
            has_hit_anything = true;
            closest_dist = last_hit.distance;
            last_hit.material = entity->material;
            *hit_ = std::move(last_hit);
        }
    }

    return has_hit_anything;
}
