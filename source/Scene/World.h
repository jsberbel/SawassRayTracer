#pragma once

#include <vector>

#include <Core/Utils.h>
#include <Scene/Geometry/Hitable.h>

class World : public IHitable
{
public:
    constexpr World();
    ~World();
    
    template <class TEntity, class... TArgs, ENABLE_IF(IS_BASE_OF(IHitable, TEntity))>
    constexpr void Add( TArgs&&... args );

    inline bool Hit( const Ray& ray, F32 tMin, F32 tMax, HitRecord& record ) const override;

private:
    std::vector<IHitable*> m_Entities;
};

constexpr World::World() = default;

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