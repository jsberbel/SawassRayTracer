// ======================================================================
// File: v3.h
// Revision: 1.0
// Creation: 11/18/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/math/v2.h"

template <class T>
union v3
{
    static_assert(std::is_trivially_destructible<T>::value);

public:
    constexpr v3() noexcept                                       : x(0), y(0), z(0) {}
    explicit constexpr v3(T _s) noexcept                          : x(_s), y(_s), z(_s) {}
    explicit constexpr v3(T _x, T _y, T _z) noexcept              : x(_x), y(_y), z(_z) {}
    explicit constexpr v3(const v2<T>& _xy, T _z = T(0)) noexcept : xy(_xy), z(_z) {}

    template <typename S>
    constexpr v3<S> cast() const { return v3<S>(S(x), S(y), S(z)); }
    
    constexpr T operator[](size_t _i) const noexcept { sws_assert(_i < 3); return (&x)[_i]; }
    constexpr T& operator[](size_t _i) noexcept      { sws_assert(_i < 3); return (&x)[_i]; }

    constexpr const v3& operator+() const noexcept;
    constexpr v3 operator-() const noexcept;

    constexpr v3& operator+=(const v3& _v) noexcept;
    constexpr v3& operator+=(T _s) noexcept;
    constexpr v3 operator+(const v3& _v) const noexcept;

    constexpr v3& operator-=(const v3& _v) noexcept;
    constexpr v3& operator-=(T _s) noexcept;
    constexpr v3 operator-(const v3& _v) const noexcept;

    constexpr v3& operator*=(const v3& _v) noexcept;
    constexpr v3& operator*=(T _s) noexcept;
    constexpr v3 operator*(const v3& _v) const noexcept;
    constexpr v3 operator*(T _s) const noexcept;

    constexpr v3& operator/=(const v3& _v) noexcept;
    constexpr v3& operator/=(T _s) noexcept;
    constexpr v3 operator/(const v3& _v) const noexcept;
    constexpr v3 operator/(T _s) const noexcept;

    constexpr b32 operator==(const v3& _v) const;
    constexpr b32 operator!=(const v3& _v) const;
    constexpr b32 operator<(const v3& _v) const;
    constexpr b32 operator<=(const v3& _v) const;
    constexpr b32 operator>(const v3& _v) const;
    constexpr b32 operator>=(const v3& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, v3& _v) noexcept;
    friend inline std::ostream& operator<<(std::ostream& _os, const v3& _v) noexcept;

    static constexpr v3 x_axis()   { return v3(T(1), T(0), T(0)); }
    static constexpr v3 y_axis()   { return v3(T(0), T(1), T(0)); }
    static constexpr v3 z_axis()   { return v3(T(0), T(0), T(1)); }
    static constexpr v3 right()    { return v3(T(1), T(0), T(0)); }
    static constexpr v3 up()       { return v3(T(0), T(1), T(0)); }
    static constexpr v3 depth()    { return v3(T(0), T(0), T(1)); }
    static constexpr v3 zero()     { return v3(T(0)); }
    static constexpr v3 one()      { return v3(T(1)); }
    static constexpr v3 infinity() { return v3(std::numeric_limits<T>::infinity()); }
    static constexpr v3 nan()      { return v3(std::numeric_limits<T>::quiet_NaN()); }

    constexpr b32 is_nan() const noexcept;
    constexpr b32 is_finite() const noexcept;
    constexpr b32 is_normalized() const noexcept;

    constexpr T get_sqrlength() const noexcept;
    constexpr T get_length() const noexcept;
    constexpr v3 get_normalized() const noexcept;
    constexpr v3 get_abs() const noexcept;
    constexpr v3 get_orthogonal() const noexcept;
    constexpr v3 get_inverse() const noexcept;
    constexpr v3 get_floor() const noexcept;
    constexpr v3 get_ceil() const noexcept;
    constexpr T get_shortest_angle(const v3& _axis = right()) const noexcept;

    inline void set(T _val) noexcept;
    inline void set_length(T _len) noexcept;
    inline void normalize() noexcept;
    inline void clear() noexcept;
    
    constexpr v2<T>& get_xy() { return xy; }
    constexpr v2<T>& get_yz() { return yz; }

    template<auto _axis>
    constexpr T get() const;

public:
    struct { T x, y, z; };
    struct { T r, g, b; };

private:
    //struct { v2<T> xy; T _0; };
    //struct { T _1; v2<T> yz; };
    u8 m_memory[sizeof(T)*3u];
};

namespace math
{
    template <class T>
    constexpr T sqrdist(const v3<T>& _a, const v3<T>& _b) noexcept
    {
        return (_b - _a).get_sqrlength();
    }

    template <class T>
    constexpr T dist(const v3<T>& _a, const v3<T>& _b) noexcept
    {
        return (_b - _a).get_length();
    }

    template <class T>
    constexpr T dot(const v3<T>& _a, const v3<T>& _b) noexcept
    {
        return _a.x*_b.x + _a.y*_b.y + _a.z*_b.z;
    }
    
    template <class T>
    constexpr v3<T> cross(const v3<T>& _a, const v3<T>& _b) noexcept
    {
        return v3<T>(
                      _a.y*_b.z - _a.z*_b.y,
                      _a.z*_b.x - _a.x*_b.z,
                      _a.x*_b.y - _a.y*_b.x
                    );
    }
}

template <class T>
constexpr const v3<T>& v3<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
constexpr v3<T> v3<T>::operator-() const noexcept
{
    return v3<T>(-x, -y, -z);
}

template <class T>
constexpr v3<T>& v3<T>::operator+=(const v3<T>& _v) noexcept
{
    x += _v.x;
    y += _v.y;
    z += _v.z;
    return *this;
}

template <class T>
constexpr v3<T>& v3<T>::operator+=(T _s) noexcept
{
    x += _s;
    y += _s;
    z += _s;
    return *this;
}

template <class T>
constexpr v3<T> v3<T>::operator+(const v3<T>& _v) const noexcept
{
    return v3<T>(x + _v.x, y + _v.y, z + _v.z);
}

template <class T>
constexpr v3<T>& v3<T>::operator-=(const v3<T>& _v) noexcept
{
    x -= _v.x;
    y -= _v.y;
    z -= _v.z;
    return *this;
}

template <class T>
constexpr v3<T>& v3<T>::operator-=(T _s) noexcept
{
    x -= _s;
    y -= _s;
    z -= _s;
    return *this;
}

template <class T>
constexpr v3<T> v3<T>::operator-(const v3<T>& _v) const noexcept
{
    return v3<T>(x - _v.x, y - _v.y, z - _v.z);
}

template <class T>
constexpr v3<T>& v3<T>::operator*=(const v3<T>& _v) noexcept
{
    x *= _v.x;
    y *= _v.y;
    z *= _v.z;
    return *this;
}

template <class T>
constexpr v3<T>& v3<T>::operator*=(T _s) noexcept
{
    x *= _s;
    y *= _s;
    z *= _s;
    return *this;
}

template <class T>
constexpr v3<T> v3<T>::operator*(const v3<T>& _v) const noexcept
{
    return v3<T>(x * _v.x, y * _v.y, z * _v.z);
}

template <class T>
constexpr v3<T> v3<T>::operator*(T _s) const noexcept
{
    return v3<T>(x * _s, y * _s, z * _s);
}

template <class T>
constexpr v3<T>& v3<T>::operator/=(const v3<T>& _v) noexcept
{
    x /= _v.x;
    y /= _v.y;
    z /= _v.z;
    return *this;
}

template <class T>
constexpr v3<T>& v3<T>::operator/=(const T _s) noexcept
{
    const f32 inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
    return *this;
}

template <class T>
constexpr v3<T> v3<T>::operator/(const v3<T>& _v) const noexcept
{
    return v3<T>(x / _v.x, y / _v.y, z / _v.z);
}

template <class T>
constexpr v3<T> v3<T>::operator/(T _s) const noexcept
{
    return v3<T>(x / _s, y / _s, z / _s);
}

template <class T>
constexpr b32 v3<T>::operator==(const v3& _v) const
{
    return x == _v.x && y == _v.y && z == _v.z;
}

template <class T>
constexpr b32 v3<T>::operator!=(const v3& _v) const
{
    return x != _v.x || y != _v.y || z != _v.z;
}

template <class T>
constexpr b32 v3<T>::operator<(const v3& _v) const
{
    return x < _v.x && y < _v.y && z < _v.z;
}

template <class T>
constexpr b32 v3<T>::operator<=(const v3& _v) const
{
    return x <= _v.x && y <= _v.y && z <= _v.z;
}

template <class T>
constexpr b32 v3<T>::operator>(const v3& _v) const
{
    return x > _v.x && y > _v.y && z > _v.z;
}

template <class T>
constexpr b32 v3<T>::operator>=(const v3& _v) const
{
    return x >= _v.x && y >= _v.y && z >= _v.z;
}

template <class T>
constexpr std::istream& operator>>(std::istream& _is, v3<T>& _v) noexcept
{
    _is >> _v.x >> _v.y >> _v.z;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const v3<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y << ' ' << _v.z;
    return _os;
}

template <class T>
constexpr b32 v3<T>::is_nan() const noexcept
{
    return math::is_nan(x) || math::is_nan(y) || math::is_nan(z);
}

template <class T>
constexpr b32 v3<T>::is_finite() const noexcept
{
    return math::is_finite(x) && math::is_finite(y) && math::is_finite(z);
}

template <class T>
constexpr b32 v3<T>::is_normalized() const noexcept
{
    const f32 sqrlength = get_sqrlength();
    return math::is_almost_null(sqrlength);
}

template <class T>
constexpr T v3<T>::get_sqrlength() const noexcept
{
    return x*x + y*y + z*z;
}

template <class T>
constexpr T v3<T>::get_length() const noexcept
{
    return math::sqrt(get_sqrlength());
}

template <class T>
constexpr v3<T> v3<T>::get_normalized() const noexcept
{
    const T inv_len = math::inv(get_length());
    return v3<T>(x * inv_len, y * inv_len, z * inv_len);
}

template <class T>
constexpr v3<T> v3<T>::get_abs() const noexcept
{
    return v3<T>(math::abs(x), math::abs(y), math::abs(z));
}

template <class T>
constexpr v3<T> v3<T>::get_orthogonal() const noexcept
{
    const T len = get_length();
    if (is_almost_equal(z, T(0)))
        return up() * len;

    const T out_x = y;
    const T out_y = z;
    const T out_z = -(x * out_x + y * out_y) / z;
    v3<T> out(out_x, out_y, out_z);
    out.set_length(len);
    return out;
}

template <class T>
constexpr v3<T> v3<T>::get_inverse() const noexcept
{
    return v3<T>(math::inv(x), math::inv(y), math::inv(z));
}

template <class T>
constexpr v3<T> v3<T>::get_floor() const noexcept
{
    return v3<T>(math::floor(x), math::floor(y), math::floor(z));
}

template <class T>
constexpr v3<T> v3<T>::get_ceil() const noexcept
{
    return v3<T>(math::ceil(x), math::ceil(y), math::ceil(z));
}

template <class T>
constexpr T v3<T>::get_shortest_angle(const v3<T>& _axis) const noexcept
{
    const T angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
    sws_assert(angle >= T(0));
    sws_assert(angle < math::Pi2<T>);
    return angle;
}

template<class T>
inline void v3<T>::set(T _val) noexcept
{
    x = y = z = _val;
}

template <class T>
inline void v3<T>::set_length(T _len) noexcept
{
    normalize();
    *this *= _len;
}

template <class T>
inline void v3<T>::normalize() noexcept
{
    const T len = get_length();
    if (len > std::numeric_limits<T>::epsilon())
        *this /= len;
    else
        clear();
}

template <class T>
inline void v3<T>::clear() noexcept
{
    x = y = z = T(0);
}

template <class T>
template<auto _axis>
constexpr T v3<T>::get() const
{
    if constexpr (_axis == decltype(_axis)(0u))
    {
        return x;
    }
    else if constexpr (_axis == decltype(_axis)(1u))
    {
        return y;
    }
    else
    {
        return z;
    }
}

template <class T>
inline v3<T> operator*(T _s, const v3<T>& _v)
{
    return _v.operator*(_s);
}

using fv3 = v3<f32>;
using dv3 = v3<f64>;
using uv3 = v3<u32>;
using sv3 = v3<s32>;
using rgb = v3<u8>;

static_assert(sizeof(fv3) == sizeof(f32) * 3u);
static_assert(sizeof(dv3) == sizeof(f64) * 3u);
static_assert(sizeof(uv3) == sizeof(u32) * 3u);
static_assert(sizeof(sv3) == sizeof(s32) * 3u);
static_assert(sizeof(rgb) == sizeof(u8) * 3u);