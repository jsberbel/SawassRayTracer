// ======================================================================
// File: ray.h
// Revision: 1.0 
// Creation: 05/21/2018 - jsberbel 
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include <core/math/algebra.h>

class Ambience;

class Ray
{
    MOVABLE_ONLY(Ray);

public:
    constexpr Ray() noexcept;
    constexpr Ray(const P3f& _origin, const V3f& _direction,
                  flt _time = 0.f, flt _t_max = sws_infinity,
                  const Ambience* _ambience = nullptr) noexcept;

    constexpr P3f operator()(flt _t) const noexcept;
    constexpr bool has_nans() const;
    friend inline std::ostream& operator<<(std::ostream& _os, const Ray &_r);

public:
    P3f origin;
    V3f direction;
    flt time;
    mutable flt t_max;
    const Ambience* ambience;
};

class RayDiff : public Ray
{
    MOVABLE_ONLY(RayDiff);

public:
    constexpr RayDiff() noexcept;
    constexpr RayDiff(const P3f& _origin, const V3f& _direction,
                      flt _time = 0.f, flt _t_max = sws_infinity,
                      const Ambience* _ambience = nullptr) noexcept;
    constexpr RayDiff(const Ray& _r);

    constexpr bool has_nans() const;
    constexpr void scale_differentials(flt _s);

public:
    bool has_differentials;
    P3f rx_origin, ry_origin;
    V3f rx_direction, ry_direction;
};

// Ray

constexpr Ray::Ray() noexcept
    : origin(0.f)
    , direction(0.f)
    , time(0.f)
    , t_max(sws_infinity)
    , ambience(nullptr)
{
}

constexpr Ray::Ray(const P3f& _origin, const V3f& _direction,
                   flt _time /*= 0.f*/, flt _t_max /*= sws_infinity*/,
                   const Ambience* _ambience /*= nullptr*/) noexcept
    : origin(_origin)
    , direction(_direction.get_normalized())
    , time(_time)
    , t_max(_t_max)
    , ambience(_ambience)
{
}

constexpr P3f Ray::operator()(flt _t) const noexcept
{
    return origin + _t * direction;
}

constexpr bool Ray::has_nans() const
{
    return (origin.has_nans() || direction.has_nans() || math::is_nan(t_max));
}

inline std::ostream& operator<<(std::ostream& _os, const Ray &_r)
{
   _os << "[o=" << _r.origin << ", d=" << _r.direction
       << ", time=" << _r.time << ", tmax=" << _r.t_max << "]";
   return _os;
}

// RayDiff

constexpr RayDiff::RayDiff() noexcept
    : Ray()
    , has_differentials(false)
{
}

constexpr RayDiff::RayDiff(const P3f& _origin, const V3f& _direction, 
                           flt _time /*= 0.f*/, flt _t_max /*= sws_infinity*/,
                           const Ambience* _ambience /*= nullptr*/) noexcept
    : Ray(_origin, _direction, _time, _t_max, _ambience)
    , has_differentials(false)
{
}

constexpr RayDiff::RayDiff(const Ray& _r) noexcept
    : Ray(_r)
    , has_differentials(false)
{
}

constexpr bool RayDiff::has_nans() const
{
    return Ray::has_nans() || (has_differentials && (rx_origin.has_nans() || ry_origin.has_nans() ||
                                                     rx_direction.has_nans() || ry_direction.has_nans()));
}

constexpr void RayDiff::scale_differentials(flt _s)
{
   rx_origin    = origin + (rx_origin - origin) * _s;
   ry_origin    = origin + (ry_origin - origin) * _s;
   rx_direction = direction + (rx_direction - direction) * _s;
   ry_direction = direction + (ry_direction - direction) * _s;
}
