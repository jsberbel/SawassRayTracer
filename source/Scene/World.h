#pragma once

#include <vector>

#include <Core/Utils.h>
#include <Scene/Geometry/Hitable.h>
#include <Engine/Camera.h>

class World
{
    MOVABLE_ONLY( World );

public:
    constexpr World();
    inline World( U32 numEntities );
    inline ~World();
    
    template <class TEntity, class... TArgs, ENABLE_IF(IS_BASE_OF(Hitable, TEntity))>
    constexpr void Add( TArgs&&... args );

    inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord* record ) const;

private:
    std::vector<Hitable*> m_Entities;
};

constexpr World::World() = default;

inline World::World( U32 numEntities )
{
    m_Entities.reserve( numEntities );
}

inline World::~World()
{
    for ( auto& entity : m_Entities )
        Utils::SafeDelete( entity );

    m_Entities.clear();
}

template <class TEntity, class... TArgs, class>
constexpr void World::Add( TArgs&&... args )
{
    m_Entities.emplace_back( new TEntity( std::forward<TArgs>( args )... ) );
}

inline bool World::Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord* hitRecord ) const
{
    HitRecord lastRecord;
    bool hitAnything = false;
    F32 closestSoFar = tMax;

    for ( const auto& entity : m_Entities )
    {
        if( entity->Hit( ray, tMin, closestSoFar, &lastRecord ) )
        {
            hitAnything = true;
            closestSoFar = lastRecord.Distance;
            lastRecord.Material = entity->m_Material;
            *hitRecord = std::move( lastRecord );
        }
    }

    return hitAnything;
}