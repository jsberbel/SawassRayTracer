// ======================================================================
// File: aabb.h
// Revision: 1.0
// Creation: 06/16/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v3.h"
#include "core/math/math.h"
#include "core/utils.h"
#include "engine/ray.h"

// Bounding Box 2D
template <typename T>
class BB2
{
    //MOVABLE_ONLY( BB2 );

public:
    constexpr BB2() = default;
    constexpr BB2(const P2<T>& _p);
    constexpr BB2(const P2<T>& _min, const P2<T>& _max);

    template <class U> explicit operator BB2<U>() const;

    constexpr V2<T> get_diagonal() const
    {
        return max - min;
    }

    constexpr T get_area() const
    {
        const V2<T> d = get_diagonal();
        return (d.x * d.y);
    }

    constexpr int get_max_extent() const
    {
        const V2<T> d = get_diagonal();
        return (d.x > d.y) ? 0 : 1;
    }

    constexpr const P2<T>& operator[](int _i) const
    {
        sws_assert(_i == 0 || _i == 1);
        return (_i == 0) ? min : max;
    }

    constexpr P2<T>& operator[](int i)
    {
        sws_assert(i == 0 || i == 1);
        return (i == 0) ? min : max;
    }

    constexpr bool operator==(const BV2<T>& b) const
    {
        return b.min == min && b.max == max;
    }

    constexpr bool operator!=(const BV2<T>& b) const
    {
        return b.min != min || b.max != max;
    }

    constexpr P2<T> lerp(const P2<T>& _t) const
    {
        return P2<T>(math::lerp(min.x, max.x, _t.x), math::lerp(min.y, max.y, _t.y));
    }

    constexpr V2<T> get_offset(const P2<T>& _p) const
    {
        const V2<T> off = _p - min;
        if (max.x > min.x) off.x /= max.x - min.x;
        if (max.y > min.y) off.y /= max.y - min.y;
        return off;
    }

    constexpr void get_bounding_sphere(P2<T>* center, flt* radius) const
    {
        *center = (min + max) / 2;
        *radius = is_inside(*center, *this) ? get_distance(*center, max) : 0;
    }

    constexpr bool is_hit(const Ray& _ray, f32 _tmin, f32 _tmax) const;

    static constexpr AABB get_surrounding_box(const AABB& _boxA, const AABB& _boxB);

public:
    P2<T> min = {};
    P2<T> max = {};
};

// Bounding Box 3D
template <typename T>
class BB3
{
    //MOVABLE_ONLY( BB3 );

public:
    constexpr BB3();
    constexpr BB3(const P3<T>& _p);
    constexpr BB3(const P3<T>& _p1, const P3<T>& _p2);

    constexpr const P3<T>& operator[](int _i) const;
    constexpr P3<T>& operator[](int _i);

    constexpr bool operator==(const BB3<T>& _bb) const;
    constexpr bool operator!=(const BB3<T>& _bb) const;

    constexpr P3<T> get_corner(int _idx) const;
    constexpr V3<T> get_diagonal() const;
    constexpr T     get_surface_area() const;
    constexpr T     get_volume() const;
    constexpr int   get_max_extent() const;
    constexpr P3<T> get_point(const P3f& _t) const;
    constexpr V3<T> get_offset(const P3<T>& _p) const;
    constexpr void  get_bounding_sphere(P3<T>* center, flt* radius) const;

    template <typename U> explicit constexpr operator BB3<U>() const;

    constexpr bool intersect(const Ray& _ray, flt* _hit_t0 = nullptr, flt* _hit_t1 = nullptr) const;
    constexpr bool intersect(const Ray& _ray, const V3f& _inv_dir, const int _dir_is_neg[3]) const;


    constexpr bool is_hit(const Ray& _ray, f32 _tmin, f32 _tmax) const;

    static constexpr AABB get_surrounding_box(const AABB& _boxA, const AABB& _boxB);

public:
    P3<T> min;
    P3<T> max;
};

using BB2f = BB2<flt>;
using BB2i = BB2<int>;
using BB3f = BB3<flt>;
using BB3i = BB3<int>;


namespace math
{
    /// <summary>
    /// Returns the bounding box that encompasses a box <paramref name="_bb"/> and a point <paramref name="_p"/>.
    /// </summary>
    template <class T>
    constexpr BB3<T> get_union(const BB3<T>& _bb, const P3<T>& _p)
    {
        return BB3<T>(P3<T>(std::min(_bb.min.x, _p.x),
                            std::min(_bb.min.y, _p.y),
                            std::min(_bb.min.z, _p.z)),
                      P3<T>(std::max(_bb.max.x, _p.x),
                            std::max(_bb.max.y, _p.y),
                            std::max(_bb.max.z, _p.z)));
    }

    /// <summary>
    /// Constructs a box that bounds the space encompassed by 2 other bounding boxes.
    /// </summary>
    template <class T>
    constexpr BB3<T> get_union(const BB3<T>& _bb1, const BB3<T>& _bb2)
    {
        return BB3<T>(P3<T>(std::min(_bb1.min.x, _bb2.min.x),
                            std::min(_bb1.min.y, _bb2.min.y),
                            std::min(_bb1.min.z, _bb2.min.z)),
                      P3<T>(std::max(_bb1.max.x, _bb2.max.x),
                            std::max(_bb1.max.y, _bb2.max.y),
                            std::max(_bb1.max.z, _bb2.max.z)));
    }

    /// <summary>
    /// Generates a bounding box as a result of the intersection of 2 bounding boxes.
    /// </summary>
    template <class T>
    constexpr BB3<T> get_intersection(const BB3<T>& _bb1, const BB3<T>& _bb2)
    {
        return BB3<T>(P3<T>(std::max(_bb1.min.x, _bb2.min.x),
                            std::max(_bb1.min.y, _bb2.min.y),
                            std::max(_bb1.min.z, _bb2.min.z)),
                      P3<T>(std::min(_bb1.max.x, _bb2.max.x),
                            std::min(_bb1.max.y, _bb2.max.y),
                            std::min(_bb1.max.z, _bb2.max.z)));
    }

    /// <summary>
    /// Checks if 2 bounding boxes overlap by seeing if their extents overlap in all coordinates.
    /// </summary>
    template <class T>
    constexpr bool are_overlapping(const BB3<T>& _bb1, const BB3<T>& _bb2)
    {
        const bool x = (_bb1.max.x >= _bb2.min.x) && (_bb1.min.x <= _bb2.max.x);
        const bool y = (_bb1.max.y >= _bb2.min.y) && (_bb1.min.y <= _bb2.max.y);
        const bool z = (_bb1.max.z >= _bb2.min.z) && (_bb1.min.z <= _bb2.max.z);
        return (x && y && z);
    }

    /// <summary>
    /// Determines if a given point <paramref name="_p"/> is inside the bounding box <paramref name="_bb"/>.
    /// </summary>
    template <class T>
    constexpr bool is_inside(const P3<T>& _p, const BB3<T>& _bb)
    {
        return (_p.x >= _bb.min.x && _p.x <= _bb.max.x &&
                _p.y >= _bb.min.y && _p.y <= _bb.max.y &&
                _p.z >= _bb.min.z && _p.z <= _bb.max.z);
    }

    /// <summary>
    /// Determines if a given point <paramref name="_p"/> is inside the bounding box <paramref name="_bb"/>.
    /// <para>Doesn’t consider points on the upper boundary to be inside the bounds.</para>
    /// <para>It is mostly useful with integer-typed bounds.</para>
    /// </summary>
    template <class T>
    constexpr bool is_xinside(const P3<T>& _p, const BB3<T>& _bb)
    {
        return (_p.x >= _bb.min.x && _p.x < _bb.max.x &&
                _p.y >= _bb.min.y && _p.y < _bb.max.y &&
                _p.z >= _bb.min.z && _p.z < _bb.max.z);
    }

    /// <summary>
    /// Returns a new bounding box as a result of padding <paramref name="_bb"/> by a constant factor  <paramref name="_delta"/> in all dimensions.
    /// </summary>
    template <class T, class U>
    constexpr BB3<T> get_expanded(const BB3<T>& _bb, U _delta)
    {
        const V3<T> delta_dir(_delta, _delta, _delta);
        return BB3<T>(_bb.min - delta_dir, _bb.max + delta_dir);
    }
}

// Bounding Box 3D

template <class T>
constexpr BB3<T>::BB3()
// Invalidate AABB on default initialization
    : min(std::numeric_limits<T>::max())
    , max(std::numeric_limits<T>::lowest())
{
}

template <class T>
constexpr BB3<T>::BB3(const P3<T>& _p)
    : min(_p)
    , max(_p)
{
}

template <class T>
constexpr BB3<T>::BB3(const P3<T>& _p1, const P3<T>& _p2)
    : min(math::min(_p1.x, _p2.x), math::min(_p1.y, _p2.y), math::min(_p1.z, _p2.z))
    , max(math::max(_p1.x, _p2.x), math::max(_p1.y, _p2.y), math::max(_p1.z, _p2.z))
{
}

template <class T>
constexpr const P3<T>& BB3<T>::operator[](int _i) const
{
    sws_assert(_i == 0 || _i == 1);
    return (_i == 0) ? min : max;
}

template <class T>
constexpr P3<T>& BB3<T>::operator[](int _i)
{
    sws_assert(_i == 0 || _i == 1);
    return (_i == 0) ? min : max;
}

template <class T>
constexpr bool BB3<T>::operator==(const BB3<T>& _bb) const
{
    return _bb.min == min && _bb.max == max;
}

template <class T>
constexpr bool BB3<T>::operator!=(const BB3<T>& _bb) const
{
    return _bb.min != min || _bb.max != max;
}

template <class T>
constexpr P3<T> BB3<T>::get_corner(int _idx) const
{
    return P3<T>(
        (*this)[(_idx & 0b001)].x,
        (*this)[(_idx & 0b010) ? 1 : 0].y,
        (*this)[(_idx & 0b100) ? 1 : 0].z
    );
}

template <class T>
constexpr V3<T> BB3<T>::get_diagonal() const
{
    return max - min;
}

template <class T>
constexpr T BB3<T>::get_surface_area() const
{
    const V3<T> d = get_diagonal();
    return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

template <class T>
constexpr T BB3<T>::get_volume() const
{
    const V3<T> d = get_diagonal();
    return d.x * d.y * d.z;
}

template <class T>
constexpr int BB3<T>::get_max_extent() const
{
    const V3<T> d = get_diagonal();
    if (d.x > d.y && d.x > d.z) return 0;
    if (d.y > d.z) return 1;
    return 2;
}

/// <summary>
/// Linearly interpolates between the corners of the box by the given amount <paramref name="_t"/> (inside the range [0,1]) for each dimension.
/// </summary>
template <class T>
constexpr P3<T> BB3<T>::get_point(const P3f& _t) const
{
    return P3<T>(math::lerp(min.x, max.x, _t.x),
                 math::lerp(min.y, max.y, _t.y),
                 math::lerp(min.z, max.z, _t.z));
}

/// <summary>
/// Returns the continuous position of a point <paramref name="_p"/> relative to the corners of the box,
/// where a point at the minimum corner has offset [0,0,0], a point at the maximum corner has offset [1,1,1], and so forth.
/// </summary>
template <class T>
constexpr V3<T> BB3<T>::get_offset(const P3<T>& _p) const
{
    const V3<T> off = _p - min;
    if (max.x > min.x) off.x /= max.x - min.x;
    if (max.y > min.y) off.y /= max.y - min.y;
    if (max.z > min.z) off.z /= max.z - min.z;
    return off;
}

template <class T>
constexpr void BB3<T>::get_bounding_sphere(P3<T>* center_, flt* radius_) const
{
    *center_ = (min + max) / 2;
    *radius_ = math::is_inside(*center_, *this) ? math::dist(*center_, max) : 0;
}

template <class T>
template <typename U>
constexpr BB3<T>::operator BB3<U>() const
{
    return BB3<U>((P3<U>)min, (P3<U>)max);
}

template <class T>
constexpr bool BB3<T>::intersect(const Ray& _ray, flt* _hit_t0 /*= nullptr*/, flt* _hit_t1 /*= nullptr*/) const
{
    
}

template <class T>
constexpr inline bool BB3<T>::intersect(const Ray& _ray, const V3f& _inv_dir, const int _dir_is_neg[3]) const
{
    
}








class AABB
{
    //MOVABLE_ONLY( AABB );

public:
    constexpr AABB() = default;
    constexpr AABB(const V3f& _min, const fv3& _max);

    constexpr void set(const fv3& _min, const fv3& _max);

    constexpr bool is_hit(const Ray& _ray, f32 _tmin, f32 _tmax) const;

    static constexpr AABB get_surrounding_box(const AABB& _boxA, const AABB& _boxB);

public:
    fv3 min = {};
    fv3 max = {};
};

constexpr AABB::AABB(const fv3& _min, const fv3& _max)
    : min(_min)
    , max(_max)
{
}

constexpr void AABB::set(const fv3& _min, const fv3& _max)
{
    min = _min;
    max = _max;
}

constexpr bool AABB::is_hit(const Ray& _ray, f32 _tmin, f32 _tmax) const
{
    for (usize i = 0u; i < 3u; ++i)
    {
        const f32 inv_dir = math::inv(_ray.direction[i]);
        const f32 t0 = math::min((min[i] - _ray.origin[i]) * inv_dir,
            (max[i] - _ray.origin[i]) * inv_dir);
        const f32 t1 = math::max((min[i] - _ray.origin[i]) * inv_dir,
            (max[i] - _ray.origin[i]) * inv_dir);

        _tmin = math::max(t0, _tmin);
        _tmax = math::min(t1, _tmax);
        if (_tmax <= _tmin)
            return false;
    }
    return true;
}

constexpr AABB AABB::get_surrounding_box(const AABB& _boxA, const AABB& _boxB)
{
    const fv3 min(math::min(_boxA.min.x, _boxB.min.x),
                  math::min(_boxA.min.y, _boxB.min.y),
                  math::min(_boxA.min.z, _boxB.min.z));

    const fv3 max(math::max(_boxA.max.x, _boxB.max.x),
                  math::max(_boxA.max.y, _boxB.max.y),
                  math::max(_boxA.max.z, _boxB.max.z));

    return AABB(min, max);
}
