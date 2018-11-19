// ======================================================================
// File: v3.h
// Revision: 1.0
// Creation: 11/18/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v2.h"

template <class T>
union look_from
{
public:
    constexpr look_from() noexcept                                       : x(0), y(0), z(0) {}
    explicit constexpr look_from(T _s) noexcept                          : x(_s), y(_s), z(_s) {}
    explicit constexpr look_from(T _x, T _y, T _z) noexcept              : x(_x), y(_y), z(_z) {}
    explicit constexpr look_from(const v2<T>& _xy, T _z = T(0)) noexcept : xy(_xy), z(_z) {}

    template <typename TT>
    constexpr look_from<TT> cast() const { return look_from<TT>(TT(x), TT(y), TT(z)); }
    
    constexpr T operator[](size_t _i) const noexcept { sws_assert(_i < 3); return (&x)[_i]; }
    constexpr T& operator[](size_t _i) noexcept      { sws_assert(_i < 3); return (&x)[_i]; }

    constexpr const look_from& operator+() const noexcept;
    constexpr look_from operator-() const noexcept;

    constexpr look_from& operator+=(const look_from& _v) noexcept;
    constexpr look_from& operator+=(T _s) noexcept;
    constexpr look_from operator+(const look_from& _v) const noexcept;

    constexpr look_from& operator-=(const look_from& _v) noexcept;
    constexpr look_from& operator-=(T _s) noexcept;
    constexpr look_from operator-(const look_from& _v) const noexcept;

    constexpr look_from& operator*=(const look_from& _v) noexcept;
    constexpr look_from& operator*=(T _s) noexcept;
    constexpr look_from operator*(const look_from& _v) const noexcept;
    constexpr look_from operator*(T _s) const noexcept;

    constexpr look_from& operator/=(const look_from& _v) noexcept;
    constexpr look_from& operator/=(T _s) noexcept;
    constexpr look_from operator/(const look_from& _v) const noexcept;
    constexpr look_from operator/(T _s) const noexcept;

    constexpr b32 operator==(const look_from& _v) const;
    constexpr b32 operator!=(const look_from& _v) const;
    constexpr b32 operator<(const look_from& _v) const;
    constexpr b32 operator<=(const look_from& _v) const;
    constexpr b32 operator>(const look_from& _v) const;
    constexpr b32 operator>=(const look_from& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, look_from& _v) noexcept;
    friend inline std::ostream& operator<<(std::ostream& _os, const look_from& _v) noexcept;

    static constexpr look_from x_axis()   { return look_from(T(1), T(0), T(0)); }
    static constexpr look_from y_axis()   { return look_from(T(0), T(1), T(0)); }
    static constexpr look_from z_axis()   { return look_from(T(0), T(0), T(1)); }
    static constexpr look_from right()    { return look_from(T(1), T(0), T(0)); }
    static constexpr look_from up()       { return look_from(T(0), T(1), T(0)); }
    static constexpr look_from depth()    { return look_from(T(0), T(0), T(1)); }
    static constexpr look_from zero()     { return look_from(T(0)); }
    static constexpr look_from one()      { return look_from(T(1)); }
    static constexpr look_from infinity() { return look_from(std::numeric_limits<T>::infinity()); }
    static constexpr look_from nan()      { return look_from(std::numeric_limits<T>::quiet_NaN()); }

    constexpr b32 is_nan() const noexcept;
    constexpr b32 is_finite() const noexcept;
    constexpr b32 is_normalized() const noexcept;

    constexpr T get_sqrlength() const noexcept;
    constexpr T get_length() const noexcept;
    constexpr look_from get_normalized() const noexcept;
    constexpr look_from get_abs() const noexcept;
    constexpr look_from get_orthogonal() const noexcept;
    constexpr look_from get_inverse() const noexcept;
    constexpr look_from get_floor() const noexcept;
    constexpr look_from get_ceil() const noexcept;
    constexpr T get_shortest_angle(const look_from& _axis = right()) const noexcept;

    inline void set_length(T _len) noexcept;
    inline void normalize() noexcept;
    inline void clear() noexcept;
    
    constexpr v2<T>& get_xy() { return xy; }
    constexpr v2<T>& get_yz() { return yz; }

public:
    struct { T x, y, z; };
    struct { T r, g, b; };

private:
    struct { v2<T> xy; T _0; };
    struct { T _1; v2<T> yz; };
};

namespace math
{
    template <class T>
    constexpr T sqrdist(const look_from<T>& _a, const look_from<T>& _b) noexcept
    {
        return (_b - _a).get_sqrlength();
    }

    template <class T>
    constexpr T dist(const look_from<T>& _a, const look_from<T>& _b) noexcept
    {
        return (_b - _a).get_length();
    }

    template <class T>
    constexpr T dot(const look_from<T>& _a, const look_from<T>& _b) noexcept
    {
        return _a.x*_b.x + _a.y*_b.y + _a.z*_b.z;
    }
    
    template <class T>
    constexpr look_from<T> cross(const look_from<T>& _a, const look_from<T>& _b) noexcept
    {
        return look_from<T>(
                      _a.y*_b.z - _a.z*_b.y,
                      _a.z*_b.x - _a.x*_b.z,
                      _a.x*_b.y - _a.y*_b.x
                    );
    }
}

template <class T>
constexpr const look_from<T>& look_from<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
constexpr look_from<T> look_from<T>::operator-() const noexcept
{
    return look_from<T>(-x, -y, -z);
}

template <class T>
constexpr look_from<T>& look_from<T>::operator+=(const look_from<T>& _v) noexcept
{
    x += _v.x;
    y += _v.y;
    z += _v.z;
    return *this;
}

template <class T>
constexpr look_from<T>& look_from<T>::operator+=(T _s) noexcept
{
    x += _s;
    y += _s;
    z += _s;
    return *this;
}

template <class T>
constexpr look_from<T> look_from<T>::operator+(const look_from<T>& _v) const noexcept
{
    return look_from<T>(x + _v.x, y + _v.y, z + _v.z);
}

template <class T>
constexpr look_from<T>& look_from<T>::operator-=(const look_from<T>& _v) noexcept
{
    x -= _v.x;
    y -= _v.y;
    z -= _v.z;
    return *this;
}

template <class T>
constexpr look_from<T>& look_from<T>::operator-=(T _s) noexcept
{
    x -= _s;
    y -= _s;
    z -= _s;
    return *this;
}

template <class T>
constexpr look_from<T> look_from<T>::operator-(const look_from<T>& _v) const noexcept
{
    return look_from<T>(x - _v.x, y - _v.y, z - _v.z);
}

template <class T>
constexpr look_from<T>& look_from<T>::operator*=(const look_from<T>& _v) noexcept
{
    x *= _v.x;
    y *= _v.y;
    z *= _v.z;
    return *this;
}

template <class T>
constexpr look_from<T>& look_from<T>::operator*=(T _s) noexcept
{
    x *= _s;
    y *= _s;
    z *= _s;
    return *this;
}

template <class T>
constexpr look_from<T> look_from<T>::operator*(const look_from<T>& _v) const noexcept
{
    return look_from<T>(x * _v.x, y * _v.y, z * _v.z);
}

template <class T>
constexpr look_from<T> look_from<T>::operator*(T _s) const noexcept
{
    return look_from<T>(x * _s, y * _s, z * _s);
}

template <class T>
constexpr look_from<T>& look_from<T>::operator/=(const look_from<T>& _v) noexcept
{
    x /= _v.x;
    y /= _v.y;
    z /= _v.z;
    return *this;
}

template <class T>
constexpr look_from<T>& look_from<T>::operator/=(const T _s) noexcept
{
    const f32 inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
    return *this;
}

template <class T>
constexpr look_from<T> look_from<T>::operator/(const look_from<T>& _v) const noexcept
{
    return look_from<T>(x / _v.x, y / _v.y, z / _v.z);
}

template <class T>
constexpr look_from<T> look_from<T>::operator/(T _s) const noexcept
{
    return look_from<T>(x / _s, y / _s, z / _s);
}

template <class T>
constexpr b32 look_from<T>::operator==(const look_from& _v) const
{
    return x == _v.x && y == _v.y && z == _v.z;
}

template <class T>
constexpr b32 look_from<T>::operator!=(const look_from& _v) const
{
    return x != _v.x || y != _v.y || z != _v.z;
}

template <class T>
constexpr b32 look_from<T>::operator<(const look_from& _v) const
{
    return x < _v.x && y < _v.y && z < _v.z;
}

template <class T>
constexpr b32 look_from<T>::operator<=(const look_from& _v) const
{
    return x <= _v.x && y <= _v.y && z <= _v.z;
}

template <class T>
constexpr b32 look_from<T>::operator>(const look_from& _v) const
{
    return x > _v.x && y > _v.y && z > _v.z;
}

template <class T>
constexpr b32 look_from<T>::operator>=(const look_from& _v) const
{
    return x >= _v.x && y >= _v.y && z >= _v.z;
}

template <class T>
constexpr std::istream& operator>>(std::istream& _is, look_from<T>& _v) noexcept
{
    _is >> _v.x >> _v.y >> _v.z;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const look_from<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y << ' ' << _v.z;
    return _os;
}

template <class T>
constexpr b32 look_from<T>::is_nan() const noexcept
{
    return math::is_nan(x) || math::is_nan(y) || math::is_nan(z);
}

template <class T>
constexpr b32 look_from<T>::is_finite() const noexcept
{
    return math::is_finite(x) && math::is_finite(y) && math::is_finite(z);
}

template <class T>
constexpr b32 look_from<T>::is_normalized() const noexcept
{
    const f32 sqrlength = get_sqrlength();
    return math::is_almost_null(sqrlength);
}

template <class T>
constexpr T look_from<T>::get_sqrlength() const noexcept
{
    return x*x + y*y + z*z;
}

template <class T>
constexpr T look_from<T>::get_length() const noexcept
{
    return math::sqrt(get_sqrlength());
}

template <class T>
constexpr look_from<T> look_from<T>::get_normalized() const noexcept
{
    const T inv_len = math::inv(get_length());
    return look_from<T>(x * inv_len, y * inv_len, z * inv_len);
}

template <class T>
constexpr look_from<T> look_from<T>::get_abs() const noexcept
{
    return look_from<T>(math::abs(x), math::abs(y), math::abs(z));
}

template <class T>
constexpr look_from<T> look_from<T>::get_orthogonal() const noexcept
{
    const T len = get_length();
    if (is_almost_equal(z, T(0)))
        return up() * len;

    const T out_x = y;
    const T out_y = z;
    const T out_z = -(x * out_x + y * out_y) / z;
    look_from<T> out(out_x, out_y, out_z);
    out.set_length(len);
    return out;
}

template <class T>
constexpr look_from<T> look_from<T>::get_inverse() const noexcept
{
    return look_from<T>(math::inv(x), math::inv(y), math::inv(z));
}

template <class T>
constexpr look_from<T> look_from<T>::get_floor() const noexcept
{
    return look_from<T>(math::floor(x), math::floor(y), math::floor(z));
}

template <class T>
constexpr look_from<T> look_from<T>::get_ceil() const noexcept
{
    return look_from<T>(math::ceil(x), math::ceil(y), math::ceil(z));
}

template <class T>
constexpr T look_from<T>::get_shortest_angle(const look_from<T>& _axis) const noexcept
{
    const T angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
    sws_assert(angle >= T(0));
    sws_assert(angle < math::Pi2<T>);
    return angle;
}

template <class T>
inline void look_from<T>::set_length(T _len) noexcept
{
    normalize();
    *this *= _len;
}

template <class T>
inline void look_from<T>::normalize() noexcept
{
    const T len = get_length();
    if (len > std::numeric_limits<T>::epsilon())
        *this /= len;
    else
        clear();
}

template <class T>
inline void look_from<T>::clear() noexcept
{
    x = y = z = T(0);
}

template <class T>
inline look_from<T> operator*(T _s, const look_from<T>& _v)
{
    return _v.operator*(_s);
}

using fv3 = look_from<f32>;
using dv3 = look_from<f64>;
using uv3 = look_from<u32>;
using sv3 = look_from<s32>;
using rgb = look_from<u8>;