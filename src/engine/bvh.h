#pragma once

#include "engine/hitable.h"

class BVH : public Hitable
{
public:
    constexpr BVH() = default;
    inline explicit BVH(Hitable** _hitables, u32 _nb_hitables, f32 _t0, f32 _t1);
    constexpr BVH(const BVH&) noexcept = delete;
    constexpr BVH& operator=(const BVH&) noexcept = delete;
    inline BVH(BVH&& _other) noexcept;
    constexpr BVH& operator=(BVH&& _other) noexcept;
    virtual inline ~BVH();

    inline bool hit(const Ray& _ray, f32 _time, f32 _tmin, f32 _tmax, Hit* hit_) const override;
    inline bool compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const override;

public:
    Hitable* left  = nullptr;
    Hitable* right = nullptr;
    AABB aabb      = {};

private:
    enum class BVHAxis { X, Y, Z };
    
    template <BVHAxis _axis>
    struct BoxCmp
    {
        inline bool operator()(Hitable* _a, Hitable* _b) const;
    };
};

inline BVH::BVH(Hitable** _hitables, u32 _nb_hitables, f32 _t0, f32 _t1)
{
    const BVHAxis axis = BVHAxis(u32(3u * util::drand_01()));
    switch (axis)
    {
        case BVHAxis::X: std::sort(_hitables, _hitables + _nb_hitables, BoxCmp<BVHAxis::X>()); break;
        case BVHAxis::Y: std::sort(_hitables, _hitables + _nb_hitables, BoxCmp<BVHAxis::Y>()); break;
        case BVHAxis::Z: std::sort(_hitables, _hitables + _nb_hitables, BoxCmp<BVHAxis::Z>()); break;
    }

    if (_nb_hitables == 1u)
    {
        left = right = _hitables[0];
    }
    else if (_nb_hitables == 2u)
    {
        left  = _hitables[0];
        right = _hitables[1];
    }
    else
    {
        const u32 half_sz = _nb_hitables / 2u;
        left  = new BVH(_hitables, half_sz, _t0, _t1);
        right = new BVH(_hitables + half_sz, _nb_hitables - half_sz, _t0, _t1);
    }

    AABB box_left, box_right;
    if (!left->compute_aabb(_t0, _t1, &box_left) ||
        !right->compute_aabb(_t0, _t1, &box_right))
    {
        util::output_to_console("No bounding box in BVH constructor.");
    }
    aabb = AABB::get_surrounding_box(box_left, box_right);
}

inline BVH::BVH(BVH&& _other) noexcept
    : left(std::exchange(_other.left, nullptr))
    , right(std::exchange(_other.right, nullptr))
    , aabb(std::move(_other.aabb))
{
}

inline constexpr BVH& BVH::operator=(BVH&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        left  = std::exchange(_other.left, nullptr);
        right = std::exchange(_other.right, nullptr);
        aabb  = std::move(_other.aabb);
    }
    return *this;
}

inline BVH::~BVH()
{
    if (BVH* l = dynamic_cast<BVH*>(left))
        util::safe_del(l);

    if (BVH* r = dynamic_cast<BVH*>(right))
        util::safe_del(r);
}

inline bool BVH::hit(const Ray& _ray, f32 _time, f32 _tmin, f32 _tmax, Hit* hit_) const
{
    if (aabb.is_hit(_ray, _tmin, _tmax))
    {
        Hit left_hit, right_hit;
        const bool is_hit_left  = left->hit(_ray, _time, _tmin, _tmax, &left_hit);
        const bool is_hit_right = right->hit(_ray, _time, _tmin, _tmax, &right_hit);
        if (is_hit_left && is_hit_right)
        {
            *hit_ = std::move((left_hit.distance < right_hit.distance) ? left_hit : right_hit);
            return true;
        }
        else if (is_hit_left)
        {
            *hit_ = std::move(left_hit);
            return true;
        }
        else if (is_hit_right)
        {
            *hit_ = std::move(right_hit);
            return true;
        }
    }
    return false;
}

inline bool BVH::compute_aabb(f32 _t0, f32 _t1, AABB* aabb_) const
{
    *aabb_ = aabb;
    return true;
}

template<BVH::BVHAxis _axis>
inline bool BVH::BoxCmp<_axis>::operator()(Hitable * _a, Hitable * _b) const
{
    AABB box_left, box_right;
    if (!_a->compute_aabb(0, 0, &box_left) ||
        !_b->compute_aabb(0, 0, &box_right))
    {
        util::output_to_console("No bounding box in BVH constructor.");
    }
    return (box_left.min.get<_axis>() - box_right.min.get<_axis>() < 0.f);
}
