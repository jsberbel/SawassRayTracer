// ======================================================================
// File: world.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/utils.h"

#include "engine/geometry/hit.h"
#include "engine/camera.h"

#include <vector>

class World
{
MOVABLE_ONLY( World );

public:
    constexpr World();
    inline World(u32 _entity_count);
    inline ~World();

    template <class TEntity, class... TArgs, ENABLE_IF(IS_BASE_OF(IHitable, TEntity))>
    constexpr void add(TArgs&&... _args);

    inline bool hit(const Ray& _ray, f32 _tmin, f32 _tmax, Hit* _hit) const;

private:
    std::vector<IHitable*> m_entities;
};

constexpr World::World() = default;

inline World::World(u32 _entity_count)
{
    m_entities.reserve(_entity_count);
}

inline World::~World()
{
    for (auto& entity : m_entities)
        utils::safe_del(entity);

    m_entities.clear();
}

template <class TEntity, class... TArgs, class>
constexpr void World::add(TArgs&&... _args)
{
    m_entities.emplace_back(new TEntity(std::forward<TArgs>(_args)...));
}

inline bool World::hit(const Ray& _ray, f32 _tmin, f32 _tmax, Hit* _hit) const
{
    Hit last_hit;
    bool has_hit_anything = false;
    f32 closest_dist = _tmax;

    for (const auto& entity : m_entities)
    {
        if (entity->hit(_ray, _tmin, closest_dist, &last_hit))
        {
            has_hit_anything = true;
            closest_dist = last_hit.distance;
            last_hit.material = entity->material;
            *_hit = std::move(last_hit);
        }
    }

    return has_hit_anything;
}
