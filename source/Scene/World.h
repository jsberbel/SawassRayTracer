#pragma once

#include <vector>

#include <Core/Utils.h>
#include <Scene/Geometry/Hitable.h>

class World
{
public:
    constexpr World();
    ~World();

    constexpr World( const World& other );
    constexpr World( World&& other );
    
    template <class TEntity, class... TArgs, ENABLE_IF(IS_BASE_OF(IHitable, TEntity))>
    constexpr void Add( TArgs&&... args );

    inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const;

private:
    std::vector<IHitable*> m_Entities;
};

constexpr World::World() = default;

constexpr World::World( const World& other )
    : m_Entities( other.m_Entities )
{
}

constexpr World::World( World&& other )
    : m_Entities( std::move( other.m_Entities ) )
{
}

World::~World()
{
    for ( auto& entity : m_Entities )
        Utils::SafeDelete( entity );

    m_Entities.clear();
}

template <class TEntity, class... TArgs, class>
constexpr void World::Add( TArgs&&... args )
{
    m_Entities.emplace_back( new TEntity(std::forward<TArgs>(args)...) );
}

inline bool World::Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const
{
    HitRecord lastRecord;
    bool hitAnything = false;
    F32 closestSoFar = tMax;

    for ( const auto& entity : m_Entities )
    {
        if( entity->Hit( ray, tMin, closestSoFar, lastRecord ) )
        {
            hitAnything = true;
            closestSoFar = lastRecord.Distance;
            lastRecord.Material = entity->Material;
            record = lastRecord;
        }
    }

    return hitAnything;
}