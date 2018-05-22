#pragma once

#include <vector>

#include "Utils.h"
#include "Hitable.h"

class World : public IHitable
{
public:
    World();
    ~World();
    
    template <class TEntity, class... TArgs, ENABLE_IF(IS_BASE_OF(IHitable, TEntity))>
    constexpr void Add(TArgs&&... args);

    bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const override;

private:
    std::vector<IHitable*> m_Entities;
};

World::World() = default;

World::~World()
{
    for ( auto& entity : m_Entities )
        delete entity;

    m_Entities.clear();
}

template <class TEntity, class... TArgs, class>
constexpr void World::Add(TArgs&&... args)
{
    m_Entities.emplace_back( new TEntity(std::forward<TArgs>(args)...) );
}

bool World::Hit(const Ray& ray, F32 tMin, F32 tMax, HitRecord& record) const
{
    HitRecord lastRecord;
    bool hitAnything = false;
    F64 closestSoFar = tMax;

    for ( const auto& entity : m_Entities )
    {
        if ( entity->Hit(ray, tMin, closestSoFar, lastRecord) )
        {
            hitAnything = true;
            closestSoFar = lastRecord.Distance;
            record = lastRecord;
        }
    }

    return hitAnything;
}