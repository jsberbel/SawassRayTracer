// ======================================================================
// File: v2.h
// Revision: 1.0
// Creation: 05/18/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/types.h"
#include "core/math/math.h"

#include <iostream>
#include <future>

template <class T>
union v2
{
public:
    constexpr v2() noexcept              : x(0), y(0) {}
    explicit constexpr v2(T _s) noexcept : x(_s), y(_s) {}
    constexpr v2(T _x, T _y) noexcept    : x(_x), y(_y) {}
    
    constexpr T operator[](size_t _i) const noexcept { sws_assert(_i < 2); return (&x)[_i]; }
    constexpr T& operator[](size_t _i) noexcept      { sws_assert(_i < 2); return (&x)[_i]; }

    constexpr const v2& operator+() const noexcept;
    constexpr v2 operator-() const noexcept;

    constexpr v2& operator+=(const v2& _v) noexcept;
    constexpr v2& operator+=(T _s) noexcept;
    constexpr v2 operator+(const v2& _v) const noexcept;

    constexpr v2& operator-=(const v2& _v) noexcept;
    constexpr v2& operator-=(T _s) noexcept;
    constexpr v2 operator-(const v2& _v) const noexcept;

    constexpr v2& operator*=(const v2& _v) noexcept;
    constexpr v2& operator*=(T _s) noexcept;
    constexpr v2 operator*(const v2& _v) const noexcept;
    constexpr v2 operator*(T _s) const noexcept;

    constexpr v2& operator/=(const v2& _v) noexcept;
    constexpr v2& operator/=(T _s) noexcept;
    constexpr v2 operator/(const v2& _v) const noexcept;
    constexpr v2 operator/(T _s) const noexcept;

    constexpr b32 operator==(const v2& _v) const;
    constexpr b32 operator!=(const v2& _v) const;
    constexpr b32 operator<(const v2& _v) const;
    constexpr b32 operator<=(const v2& _v) const;
    constexpr b32 operator>(const v2& _v) const;
    constexpr b32 operator>=(const v2& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, v2& _v) noexcept;
    friend inline std::ostream& operator<<(std::ostream& _os, const v2& _v) noexcept;

    static constexpr v2 x_axis()   { return v2(T(1), T(0)); }
    static constexpr v2 y_axis()   { return v2(T(0), T(1)); }
    static constexpr v2 up()       { return v2(T(0), T(1)); }
    static constexpr v2 down()     { return v2(T(0), T(-1)); }
    static constexpr v2 right()    { return v2(T(1), T(0)); }
    static constexpr v2 left()     { return v2(T(-1), T(0)); }
    static constexpr v2 zero()     { return v2(T(0)); }
    static constexpr v2 one()      { return v2(T(1)); }
    static constexpr v2 infinity() { return v2(std::numeric_limits<T>::infinity()); }
    static constexpr v2 nan()      { return v2(std::numeric_limits<T>::quiet_NaN()); }

    constexpr b32 is_nan() const noexcept;
    constexpr b32 is_finite() const noexcept;
    constexpr b32 is_normalized() const noexcept;

    constexpr T get_sqrlength() const noexcept;
    constexpr T get_length() const noexcept;
    constexpr v2 get_normalized() const noexcept;
    constexpr v2 get_abs() const noexcept;
    constexpr v2 get_orthogonal() const noexcept;
    constexpr v2 get_inverse() const noexcept;
    constexpr v2 get_floor() const noexcept;
    constexpr v2 get_ceil() const noexcept;
    constexpr T get_angle(const v2& _axis = right()) const noexcept;
    constexpr T get_shortest_angle(const v2& _axis = right()) const noexcept;
    
    inline void set_length(T _len) noexcept;
    inline void normalize() noexcept;
    inline void clear() noexcept;

public:
    struct { T x, y; };
    struct { T u, v; };

private:
    u8 m_memory[sizeof(T)*2u];
};

namespace math
{
    template <class T>
    constexpr T sqrdist(const v2<T>& _a, const v2<T>& _b) noexcept
    {
        return (_b - _a).get_sqrlength();
    }

    template <class T>
    constexpr T dist(const v2<T>& _a, const v2<T>& _b) noexcept
    {
        return (_b - _a).get_length();
    }

    template <class T>
    constexpr T dot(const v2<T>& _a, const v2<T>& _b) noexcept
    {
        return _a.x*_b.x + _a.y*_b.y;
    }
    
    template <class T>
    constexpr T cross(const v2<T>& _a, const v2<T>& _b) noexcept
    {
        return _a.x*_b.y - _a.y*_b.x;
    }
}

template <class T>
constexpr const v2<T>& v2<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
constexpr v2<T> v2<T>::operator-() const noexcept
{
    return v2<T>(-x, -y);
}

template <class T>
constexpr v2<T>& v2<T>::operator+=(const v2<T>& _v) noexcept
{
    x += _v.x;
    y += _v.y;
    return *this;
}

template <class T>
constexpr v2<T>& v2<T>::operator+=(T _s) noexcept
{
    x += _s;
    y += _s;
    return *this;
}

template <class T>
constexpr v2<T> v2<T>::operator+(const v2<T>& _v) const noexcept
{
    return v2<T>(x + _v.x, y + _v.y);
}

template <class T>
constexpr v2<T>& v2<T>::operator-=(const v2<T>& _v) noexcept
{
    x -= _v.x;
    y -= _v.y;
    return *this;
}

template <class T>
constexpr v2<T>& v2<T>::operator-=(T _s) noexcept
{
    x -= _s;
    y -= _s;
    return *this;
}

template <class T>
constexpr v2<T> v2<T>::operator-(const v2<T>& _v) const noexcept
{
    return v2<T>(x - _v.x, y - _v.y);
}

template <class T>
constexpr v2<T>& v2<T>::operator*=(const v2<T>& _v) noexcept
{
    x *= _v.x;
    y *= _v.y;
    return *this;
}

template <class T>
constexpr v2<T>& v2<T>::operator*=(T _s) noexcept
{
    x *= _s;
    y *= _s;
    return *this;
}

template <class T>
constexpr v2<T> v2<T>::operator*(const v2<T>& _v) const noexcept
{
    return v2<T>(x * _v.x, y * _v.y);
}

template <class T>
constexpr v2<T> v2<T>::operator*(T _s) const noexcept
{
    return v2<T>(x * _s, y * _s);
}

template <class T>
constexpr v2<T>& v2<T>::operator/=(const v2<T>& _v) noexcept
{
    x /= _v.x;
    y /= _v.y;
    return *this;
}

template <class T>
constexpr v2<T>& v2<T>::operator/=(const T _s) noexcept
{
    const f32 inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    return *this;
}

template <class T>
constexpr v2<T> v2<T>::operator/(const v2<T>& _v) const noexcept
{
    return v2<T>(x / _v.x, y / _v.y);
}

template <class T>
constexpr v2<T> v2<T>::operator/(T _s) const noexcept
{
    return v2<T>(x / _s, y / _s);
}

template <class T>
constexpr b32 v2<T>::operator==(const v2& _v) const
{
    return x == _v.x && y == _v.y;
}

template <class T>
constexpr b32 v2<T>::operator!=(const v2& _v) const
{
    return x != _v.x || y != _v.y;
}

template <class T>
constexpr b32 v2<T>::operator<(const v2& _v) const
{
    return x < _v.x && y < _v.y;
}

template <class T>
constexpr b32 v2<T>::operator<=(const v2& _v) const
{
    return x <= _v.x && y <= _v.y;
}

template <class T>
constexpr b32 v2<T>::operator>(const v2& _v) const
{
    return x > _v.x && y > _v.y;
}

template <class T>
constexpr b32 v2<T>::operator>=(const v2& _v) const
{
    return x >= _v.x && y >= _v.y;
}

template <class T>
constexpr std::istream& operator>>(std::istream& _is, v2<T>& _v) noexcept
{
    _is >> _v.x >> _v.y;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const v2<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y;
    return _os;
}

template <class T>
constexpr b32 v2<T>::is_nan() const noexcept
{
    return math::is_nan(x) || math::is_nan(y);
}

template <class T>
constexpr b32 v2<T>::is_finite() const noexcept
{
    return math::is_finite(x) && math::is_finite(y);
}

template <class T>
constexpr b32 v2<T>::is_normalized() const noexcept
{
    const f32 sqrlength = get_sqrlength();
    return math::is_almost_null(sqrlength);
}

template <class T>
constexpr T v2<T>::get_sqrlength() const noexcept
{
    return x * x + y * y;
}

template <class T>
constexpr T v2<T>::get_length() const noexcept
{
    return math::sqrt(get_sqrlength());
}

template <class T>
constexpr v2<T> v2<T>::get_normalized() const noexcept
{
    const T inv_len = math::inv(get_length());
    return v2<T>(x * inv_len, y * inv_len);
}

template <class T>
constexpr v2<T> v2<T>::get_abs() const noexcept
{
    return v2<T>(math::abs(x), math::abs(y));
}

template <class T>
constexpr v2<T> v2<T>::get_orthogonal() const noexcept
{
    return v2<T>(-y, x);
}

template <class T>
constexpr v2<T> v2<T>::get_inverse() const noexcept
{
    return v2<T>(math::inv(x), math::inv(y));
}

template <class T>
constexpr v2<T> v2<T>::get_floor() const noexcept
{
    return v2<T>(math::floor(x), math::floor(y));
}

template <class T>
constexpr v2<T> v2<T>::get_ceil() const noexcept
{
    return v2<T>(math::ceil(x), math::ceil(y));
}

template <class T>
constexpr T v2<T>::get_angle(const v2<T>& _axis) const noexcept
{
    const T x = math::dot(_axis, *this);
    const T y = math::dot(_axis.get_orthogonal(), *this);
    const T angle = math::atan2(y, x);
    return angle;
}

template <class T>
constexpr T v2<T>::get_shortest_angle(const v2<T>& _axis) const noexcept
{
    const T angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
    sws_assert(angle >= T(0));
    sws_assert(angle < math::Pi2<T>);
    return angle;
}

template <class T>
inline void v2<T>::set_length(T _len) noexcept
{
    normalize();
    *this *= _len;
}

template <class T>
inline void v2<T>::normalize() noexcept
{
    const T len = get_length();
    if (len > std::numeric_limits<T>::epsilon())
        *this /= len;
    else
        clear();
}

template <class T>
inline void v2<T>::clear() noexcept
{
    x = y = T(0);
}

template <class T>
inline v2<T> operator*(T _s, const v2<T>& _v)
{
    return _v.operator*(_s);
}

using fv2 = v2<f32>;
using dv2 = v2<f64>;
using uv2 = v2<u32>;
using sv2 = v2<s32>;