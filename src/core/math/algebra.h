// ======================================================================
// File: v2.h
// Revision: 1.0
// Creation: 05/18/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/types.h"
#include "core/math/math.h"
#include "core/logger.h"

#include <iostream>
#include <future>

template <class T> union V2;
template <class T> union V3;
template <class T> union P2;
template <class T> union P3;
template <class T> union N2;
template <class T> union N3;

template <class T>
union V2 // Vector 2D
{
    static_assert(std::is_trivially_destructible<T>::value);
public:
    constexpr V2()                         : x(0), y(0)       {}
    explicit constexpr V2(T _s)            : x(_s), y(_s)     { sws_assert(!is_nan()); }
    explicit constexpr V2(T _x, T _y)      : x(_x), y(_y)     { sws_assert(!is_nan()); }
    explicit constexpr V2(const P2<T> &_p) : x(_p.x), y(_p.y) { sws_assert(!is_nan()); }
    explicit constexpr V2(const P3<T> &_p) : x(_p.x), y(_p.y) { sws_assert(!is_nan()); }
    
    constexpr T operator[](size_t _i) const noexcept { sws_assert(_i < 2); return (&x)[_i]; }
    constexpr T& operator[](size_t _i) noexcept      { sws_assert(_i < 2); return (&x)[_i]; }

    constexpr const V2& operator+() const noexcept;
    constexpr V2 operator-() const noexcept;

    constexpr V2& operator+=(const V2& _v) noexcept;
    constexpr V2 operator+(const V2& _v) const noexcept;

    constexpr V2& operator-=(const V2& _v) noexcept;
    constexpr V2 operator-(const V2& _v) const noexcept;

    constexpr V2& operator*=(const V2& _v) noexcept;
    constexpr V2 operator*(const V2& _v) const noexcept;
    template <class S> constexpr V2& operator*=(S _s) noexcept;
    template <class S> constexpr V2 operator*(S _s) const noexcept;

    constexpr V2& operator/=(T _s) noexcept;
    constexpr V2 operator/(T _s) const noexcept;

    constexpr bool operator==(const V2& _v) const;
    constexpr bool operator!=(const V2& _v) const;
    constexpr bool operator<(const V2& _v) const;
    constexpr bool operator<=(const V2& _v) const;
    constexpr bool operator>(const V2& _v) const;
    constexpr bool operator>=(const V2& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, V2& _v) noexcept;
    friend inline std::ostream& operator<<(std::ostream& _os, const V2& _v) noexcept;

    static constexpr V2 x_axis()   { return V2(T(1), T(0)); }
    static constexpr V2 y_axis()   { return V2(T(0), T(1)); }
    static constexpr V2 up()       { return V2(T(0), T(1)); }
    static constexpr V2 down()     { return V2(T(0), T(-1)); }
    static constexpr V2 right()    { return V2(T(1), T(0)); }
    static constexpr V2 left()     { return V2(T(-1), T(0)); }
    static constexpr V2 zero()     { return V2(T(0)); }
    static constexpr V2 one()      { return V2(T(1)); }
    static constexpr V2 infinity() { return V2(std::numeric_limits<T>::infinity()); }
    static constexpr V2 nan()      { return V2(std::numeric_limits<T>::quiet_NaN()); }

    constexpr bool is_nan() const noexcept;
    constexpr bool is_finite() const noexcept;
    constexpr bool is_normalized() const noexcept;

    constexpr fx get_sqrlength() const noexcept;
    constexpr fx get_length() const noexcept;
    constexpr V2 get_normalized() const noexcept;
    constexpr V2 get_abs() const noexcept;
    constexpr V2 get_orthogonal() const noexcept;
    constexpr V2 get_inverse() const noexcept;
    constexpr V2 get_floor() const noexcept;
    constexpr V2 get_ceil() const noexcept;
    constexpr fx get_angle(const V2& _axis = right()) const noexcept;
    constexpr fx get_shortest_angle(const V2& _axis = right()) const noexcept;
    
    inline void set_length(fx _len) noexcept;
    inline void normalize() noexcept;
    inline void clear() noexcept;

public:
    struct { T x, y; };
    struct { T u, v; };

private:
    byte m_memory[sizeof(T) * 2];
};

template <class T>
union V3 // Vector 3D
{
    static_assert(std::is_trivially_destructible<T>::value);
public:
    constexpr v3()                         : x(0), y(0)       {}
    explicit constexpr v3(T _s)            : x(_s), y(_s)     { sws_assert(!is_nan()); }
    explicit constexpr v3(T _x, T _y)      : x(_x), y(_y)     { sws_assert(!is_nan()); }
    explicit constexpr v3(const P2<T> &_p) : x(_p.x), y(_p.y) { sws_assert(!is_nan()); }
    explicit constexpr v3(const P3<T> &_p) : x(_p.x), y(_p.y) { sws_assert(!is_nan()); }
    
    constexpr T operator[](size_t _i) const noexcept { sws_assert(_i < 2); return (&x)[_i]; }
    constexpr T& operator[](size_t _i) noexcept      { sws_assert(_i < 2); return (&x)[_i]; }

    constexpr const V2& operator+() const noexcept;
    constexpr V2 operator-() const noexcept;

    constexpr V2& operator+=(const V2& _v) noexcept;
    constexpr V2 operator+(const V2& _v) const noexcept;

    constexpr V2& operator-=(const V2& _v) noexcept;
    constexpr V2 operator-(const V2& _v) const noexcept;

    constexpr V2& operator*=(const V2& _v) noexcept;
    constexpr V2 operator*(const V2& _v) const noexcept;
    template <class S> constexpr V2& operator*=(S _s) noexcept;
    template <class S> constexpr V2 operator*(S _s) const noexcept;

    constexpr V2& operator/=(T _s) noexcept;
    constexpr V2 operator/(T _s) const noexcept;

    constexpr bool operator==(const V2& _v) const;
    constexpr bool operator!=(const V2& _v) const;
    constexpr bool operator<(const V2& _v) const;
    constexpr bool operator<=(const V2& _v) const;
    constexpr bool operator>(const V2& _v) const;
    constexpr bool operator>=(const V2& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, V2& _v) noexcept;
    friend inline std::ostream& operator<<(std::ostream& _os, const V2& _v) noexcept;

    static constexpr V2 x_axis()   { return V2(T(1), T(0)); }
    static constexpr V2 y_axis()   { return V2(T(0), T(1)); }
    static constexpr V2 up()       { return V2(T(0), T(1)); }
    static constexpr V2 down()     { return V2(T(0), T(-1)); }
    static constexpr V2 right()    { return V2(T(1), T(0)); }
    static constexpr V2 left()     { return V2(T(-1), T(0)); }
    static constexpr V2 zero()     { return V2(T(0)); }
    static constexpr V2 one()      { return V2(T(1)); }
    static constexpr V2 infinity() { return V2(std::numeric_limits<T>::infinity()); }
    static constexpr V2 nan()      { return V2(std::numeric_limits<T>::quiet_NaN()); }

    constexpr bool is_nan() const noexcept;
    constexpr bool is_finite() const noexcept;
    constexpr bool is_normalized() const noexcept;

    constexpr fx get_sqrlength() const noexcept;
    constexpr fx get_length() const noexcept;
    constexpr V2 get_normalized() const noexcept;
    constexpr V2 get_abs() const noexcept;
    constexpr V2 get_orthogonal() const noexcept;
    constexpr V2 get_inverse() const noexcept;
    constexpr V2 get_floor() const noexcept;
    constexpr V2 get_ceil() const noexcept;
    constexpr fx get_angle(const V2& _axis = right()) const noexcept;
    constexpr fx get_shortest_angle(const V2& _axis = right()) const noexcept;
    
    inline void set_length(fx _len) noexcept;
    inline void normalize() noexcept;
    inline void clear() noexcept;

public:
    struct { T x, y; };
    struct { T u, v; };

private:
    byte m_memory[sizeof(T) * 2];
};


namespace math
{
    template <class T>
    constexpr T sqrdist(const V2<T>& _a, const V2<T>& _b) noexcept
    {
        return (_b - _a).get_sqrlength();
    }

    template <class T>
    constexpr T dist(const V2<T>& _a, const V2<T>& _b) noexcept
    {
        return (_b - _a).get_length();
    }

    template <class T>
    constexpr T dot(const V2<T>& _a, const V2<T>& _b) noexcept
    {
        return _a.x*_b.x + _a.y*_b.y;
    }
    
    template <class T>
    constexpr T cross(const V2<T>& _a, const V2<T>& _b) noexcept
    {
        return _a.x*_b.y - _a.y*_b.x;
    }
}

template <class T>
constexpr const V2<T>& V2<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
constexpr V2<T> V2<T>::operator-() const noexcept
{
    return V2<T>(-x, -y);
}

template <class T>
constexpr V2<T>& V2<T>::operator+=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.is_nan());
    x += _v.x;
    y += _v.y;
    return *this;
}

template <class T>
constexpr V2<T> V2<T>::operator+(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.is_nan());
    return V2<T>(x + _v.x, y + _v.y);
}

template <class T>
constexpr V2<T>& V2<T>::operator-=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.is_nan());
    x -= _v.x;
    y -= _v.y;
    return *this;
}

template <class T>
constexpr V2<T> V2<T>::operator-(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.is_nan());
    return V2<T>(x - _v.x, y - _v.y);
}

template <class T>
constexpr V2<T>& V2<T>::operator*=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.is_nan());
    x *= _v.x;
    y *= _v.y;
    return *this;
}

template <class T>
constexpr V2<T> V2<T>::operator*(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.is_nan());
    return V2<T>(x * _v.x, y * _v.y);
}

template <class T>
template <class S>
constexpr V2<T>& V2<T>::operator*=(S _s) noexcept
{
    sws_assert(!std::isnan(_s));
    x *= _s;
    y *= _s;
    return *this;
}

template <class T>
template <class S>
constexpr V2<T> V2<T>::operator*(S _s) const noexcept
{
    sws_assert(!std::isnan(_s));
    return V2<T>(x * _s, y * _s);
}

//template <class T>
//constexpr V2<T>& V2<T>::operator/=(const V2<T>& _v) noexcept
//{
//    sws_assert(!_v.is_nan());
//    x /= _v.x;
//    y /= _v.y;
//    return *this;
//}

template <class T>
constexpr V2<T>& V2<T>::operator/=(const T _s) noexcept
{
    sws_verify(_s != T(0));
    const fx inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    return *this;
}

//template <class T>
//constexpr V2<T> V2<T>::operator/(const V2<T>& _v) const noexcept
//{
//    return V2<T>(x / _v.x, y / _v.y);
//}

template <class T>
constexpr V2<T> V2<T>::operator/(T _s) const noexcept
{
    sws_verify(_s != T(0));
    const fx inv_s = math::inv(_s);
    return V2<T>(x * inv_s, y * inv_s);
}

template <class T>
constexpr bool V2<T>::operator==(const V2& _v) const
{
    return x == _v.x && y == _v.y;
}

template <class T>
constexpr bool V2<T>::operator!=(const V2& _v) const
{
    return x != _v.x || y != _v.y;
}

template <class T>
constexpr bool V2<T>::operator<(const V2& _v) const
{
    return x < _v.x && y < _v.y;
}

template <class T>
constexpr bool V2<T>::operator<=(const V2& _v) const
{
    return x <= _v.x && y <= _v.y;
}

template <class T>
constexpr bool V2<T>::operator>(const V2& _v) const
{
    return x > _v.x && y > _v.y;
}

template <class T>
constexpr bool V2<T>::operator>=(const V2& _v) const
{
    return x >= _v.x && y >= _v.y;
}

template <class T>
constexpr std::istream& operator>>(std::istream& _is, V2<T>& _v) noexcept
{
    _is >> _v.x >> _v.y;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const V2<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y;
    return _os;
}

template <class T>
constexpr bool V2<T>::is_nan() const noexcept
{
    return math::is_nan(x) || math::is_nan(y);
}

template <class T>
constexpr bool V2<T>::is_finite() const noexcept
{
    return math::is_finite(x) && math::is_finite(y);
}

template <class T>
constexpr bool V2<T>::is_normalized() const noexcept
{
    const fx sqrlength = get_sqrlength();
    return math::is_almost_null(sqrlength);
}

template <class T>
constexpr fx V2<T>::get_sqrlength() const noexcept
{
    return x * x + y * y;
}

template <class T>
constexpr fx V2<T>::get_length() const noexcept
{
    return math::sqrt(get_sqrlength());
}

template <class T>
constexpr V2<T> V2<T>::get_normalized() const noexcept
{
    const T inv_len = math::inv(get_length());
    return V2<T>(x * inv_len, y * inv_len);
}

template <class T>
constexpr V2<T> V2<T>::get_abs() const noexcept
{
    return V2<T>(math::abs(x), math::abs(y));
}

template <class T>
constexpr V2<T> V2<T>::get_orthogonal() const noexcept
{
    return V2<T>(-y, x);
}

template <class T>
constexpr V2<T> V2<T>::get_inverse() const noexcept
{
    return V2<T>(math::inv(x), math::inv(y));
}

template <class T>
constexpr V2<T> V2<T>::get_floor() const noexcept
{
    return V2<T>(math::floor(x), math::floor(y));
}

template <class T>
constexpr V2<T> V2<T>::get_ceil() const noexcept
{
    return V2<T>(math::ceil(x), math::ceil(y));
}

template <class T>
constexpr fx V2<T>::get_angle(const V2<T>& _axis) const noexcept
{
    const fx x = math::dot(_axis, *this);
    const fx y = math::dot(_axis.get_orthogonal(), *this);
    const fx angle = math::atan2(y, x);
    return angle;
}

template <class T>
constexpr fx V2<T>::get_shortest_angle(const V2<T>& _axis) const noexcept
{
    const fx angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
    sws_assert(angle >= 0.f);
    sws_assert(angle < math::Pi2f);
    return angle;
}

template <class T>
inline void V2<T>::set_length(fx _len) noexcept
{
    normalize();
    *this *= _len;
}

template <class T>
inline void V2<T>::normalize() noexcept
{
    const fx len = get_length();
    if (len > std::numeric_limits<T>::epsilon())
        *this /= len;
    else
        clear();
}

template <class T>
inline void V2<T>::clear() noexcept
{
    x = y = T(0);
}

template <class T>
inline V2<T> operator*(T _s, const V2<T>& _v)
{
    return _v.operator*(_s);
}

using V2f = V2<fx>;
using V2i = V2<ix>;