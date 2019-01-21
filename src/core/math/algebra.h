// ======================================================================
// File: v2.h
// Revision: 1.0
// Creation: 05/18/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/types.h"
#include "core/math/math.h"
#include "core/check.h"

#include <iostream>
#include <future>


template <class T> union V2;
template <class T> union V3;
template <class T> union P2;
template <class T> union P3;


template <class T>
union V2 // Vector 2D
{
    static_assert(std::is_trivially_destructible<T>::value);
public:
    constexpr V2()                         : x(0), y(0)       {}
    explicit constexpr V2(T _s)            : x(_s), y(_s)     { sws_assert(!has_nans()); }
    explicit constexpr V2(T _x, T _y)      : x(_x), y(_y)     { sws_assert(!has_nans()); }
    explicit constexpr V2(const P2<T> &_v) : x(_v.x), y(_v.y) { sws_assert(!has_nans()); }
    explicit constexpr V2(const P3<T> &_v) : x(_v.x), y(_v.y) { sws_assert(!has_nans()); }

    constexpr V2<T>& operator=(const V2<T> &_v);
    
    constexpr T  operator[](usize _i) const noexcept { sws_assert(_i < 2); return (&x)[_i]; }
    constexpr T& operator[](usize _i) noexcept       { sws_assert(_i < 2); return (&x)[_i]; }

    constexpr const V2& operator+() const noexcept;
    constexpr V2        operator-() const noexcept;

    constexpr V2  operator+ (const V2& _v) const noexcept;
    constexpr V2& operator+=(const V2& _v) noexcept;

    constexpr V2  operator- (const V2& _v) const noexcept;
    constexpr V2& operator-=(const V2& _v) noexcept;

    constexpr V2  operator* (const V2& _v) const noexcept;
    constexpr V2& operator*=(const V2& _v) noexcept;

    template <class S> constexpr V2  operator* (S _s) const noexcept;
    template <class S> constexpr V2& operator*=(S _s) noexcept;

    template <class S> constexpr V2  operator/ (S _s) const noexcept;
    template <class S> constexpr V2& operator/=(S _s) noexcept;

    constexpr bool operator==(const V2& _v) const;
    constexpr bool operator!=(const V2& _v) const;
    constexpr bool operator< (const V2& _v) const;
    constexpr bool operator<=(const V2& _v) const;
    constexpr bool operator> (const V2& _v) const;
    constexpr bool operator>=(const V2& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, V2& _v)       noexcept;
    friend inline    std::ostream& operator<<(std::ostream& _os, const V2& _v) noexcept;

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

    constexpr bool has_nans()      const noexcept;
    constexpr bool is_finite()     const noexcept;
    constexpr bool is_normalized() const noexcept;

    constexpr flt get_sqrlength()                               const noexcept;
    constexpr flt get_length()                                  const noexcept;
    constexpr V2 get_normalized()                               const noexcept;
    constexpr V2 get_abs()                                      const noexcept;
    constexpr V2 get_orthogonal()                               const noexcept;
    constexpr V2 get_inverse()                                  const noexcept;
    constexpr V2 get_floor()                                    const noexcept;
    constexpr V2 get_ceil()                                     const noexcept;
    constexpr flt get_angle(const V2& _axis = right())          const noexcept;
    constexpr flt get_shortest_angle(const V2& _axis = right()) const noexcept;

    inline void set(T _s)          noexcept;
    inline void set_length(flt _len) noexcept;
    inline void normalize()          noexcept;
    inline void clear()              noexcept;

public:
    struct { T x, y; };
    struct { T u, v; };

private:
    byte m_memory[sizeof(T) * 2];
};

using V2f = V2<flt>;
using V2i = V2<int>;


template <class T>
union V3 // Vector 3D
{
    static_assert(std::is_trivially_destructible<T>::value);
public:
    constexpr V3()                          : x(0), y(0), z(0)          {}
    explicit constexpr V3(T _s)             : x(_s), y(_s), z(_s)       { sws_assert(!has_nans()); }
    explicit constexpr V3(T _x, T _y, T _z) : x(_x), y(_y), z(_z)       { sws_assert(!has_nans()); }
    explicit constexpr V3(const V3<T> &_v)  : x(_v.x), y(_v.y), z(_v.z) { sws_assert(!has_nans()); }
    explicit constexpr V3(const P3<T> &_v)  : x(_v.x), y(_v.y), z(_v.z) { sws_assert(!has_nans()); }

    constexpr V3<T>& operator=(const V3<T> &_v);
    
    constexpr T  operator[](usize _i) const noexcept { sws_assert(_i < 3); return (&x)[_i]; }
    constexpr T& operator[](usize _i) noexcept       { sws_assert(_i < 3); return (&x)[_i]; }

    constexpr const V3& operator+() const noexcept;
    constexpr V3        operator-() const noexcept;

    constexpr V3  operator+ (const V3& _v) const noexcept;
    constexpr V3& operator+=(const V3& _v) noexcept;

    constexpr V3  operator- (const V3& _v) const noexcept;
    constexpr V3& operator-=(const V3& _v) noexcept;

    constexpr V3  operator* (const V3& _v) const noexcept;
    constexpr V3& operator*=(const V3& _v) noexcept;

    template <class S> constexpr V3  operator* (S _s) const noexcept;
    template <class S> constexpr V3& operator*=(S _s) noexcept;

    template <class S> constexpr V3  operator/ (S _s) const noexcept;
    template <class S> constexpr V3& operator/=(S _s) noexcept;

    constexpr bool operator==(const V3& _v) const;
    constexpr bool operator!=(const V3& _v) const;
    constexpr bool operator< (const V3& _v) const;
    constexpr bool operator<=(const V3& _v) const;
    constexpr bool operator> (const V3& _v) const;
    constexpr bool operator>=(const V3& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, V3& _v)       noexcept;
    friend inline    std::ostream& operator<<(std::ostream& _os, const V3& _v) noexcept;

    static constexpr V3 x_axis()    { return V3(T(1), T(0), T(0)); }
    static constexpr V3 y_axis()    { return V3(T(0), T(1), T(0)); }
    static constexpr V3 z_axis()    { return V3(T(0), T(0), T(1)); }
    static constexpr V3 right()     { return V3(T(1), T(0), T(0)); }
    static constexpr V3 up()        { return V3(T(0), T(1), T(0)); }
    static constexpr V3 depth()     { return V3(T(0), T(0), T(1)); }
    static constexpr V3 zero()      { return V3(T(0)); }
    static constexpr V3 one()       { return V3(T(1)); }
    static constexpr V3 infinity()  { return V3(std::numeric_limits<T>::infinity()); }
    static constexpr V3 nan()       { return V3(std::numeric_limits<T>::quiet_NaN()); }

    constexpr bool has_nans()       const noexcept;
    constexpr bool is_finite()      const noexcept;
    constexpr bool is_normalized()  const noexcept;

    constexpr flt get_sqrlength()                               const noexcept;
    constexpr flt get_length()                                  const noexcept;
    constexpr V3 get_normalized()                               const noexcept;
    constexpr V3 get_abs()                                      const noexcept;
    constexpr V3 get_orthogonal()                               const noexcept;
    constexpr V3 get_inverse()                                  const noexcept;
    constexpr V3 get_floor()                                    const noexcept;
    constexpr V3 get_ceil()                                     const noexcept;
    constexpr V3 get_clamped(T _min, T _max)                    const noexcept;
    constexpr flt get_shortest_angle(const V3& _axis = right()) const noexcept;

    inline void set(T _s)          noexcept;
    inline void set_length(flt _len) noexcept;
    inline void normalize()          noexcept;
    inline void clear()              noexcept;

    template <auto _axis> constexpr T get() const;

public:
    struct { T x, y, z; };
    struct { T r, g, b; };

private:
    u8 m_memory[sizeof(T) * 3];
};

using V3f = V3<flt>;
using V3i = V3<int>;
using RGB = V3<u8>;

template<class T> using N3 = V3<T>;
using N3f = N3<flt>;
using N3i = N3<int>;


template <class T>
union P2 // Point 2D
{
    static_assert(std::is_trivially_destructible<T>::value);
public:
    constexpr P2()                         : x(0), y(0)       {}
    explicit constexpr P2(T _s)            : x(_s), y(_s)     { sws_assert(!has_nans()); }
    explicit constexpr P2(T _x, T _y)      : x(_x), y(_y)     { sws_assert(!has_nans()); }
    explicit constexpr P2(const P2<T>& _p) : x(_p.x), y(_p.y) { sws_assert(!has_nans()); }
    explicit constexpr P2(const P3<T>& _p) : x(_p.x), y(_p.y) { sws_assert(!has_nans()); }
    template <class U> explicit constexpr P2(const P2<U>& _p) : x(T(_p.x)), y(T(_p.y)) { sws_assert(!has_nans()); }
    template <class U> explicit constexpr P2(const V2<U>& _p) : x(T(_p.x)), y(T(_p.y)) { sws_assert(!has_nans()); }

    constexpr P2<T>& operator=(const P2<T> &_p);

    template <class U> explicit constexpr operator V2<U>() const { return V2<U>(x,y); }
    
    constexpr T  operator[](usize _i) const noexcept { sws_assert(_i < 2); return (&x)[_i]; }
    constexpr T& operator[](usize _i) noexcept       { sws_assert(_i < 2); return (&x)[_i]; }

    constexpr P2  operator-() const noexcept;

    constexpr P2<T>  operator+ (const P2<T>& _p) const noexcept;
    constexpr P2<T>  operator+ (const V2<T>& _v) const noexcept;
    constexpr P2<T>& operator+=(const P2<T>& _p) noexcept;
    constexpr P2<T>& operator+=(const V2<T>& _v) noexcept;

    constexpr V2<T>  operator- (const P2<T>& _p) const noexcept;
    constexpr P2<T>  operator- (const V2<T>& _v) const noexcept;
    constexpr P2<T>& operator-=(const V2<T>& _v) noexcept;

    template <class S> constexpr P2  operator* (S _s) const noexcept;
    template <class S> constexpr P2& operator*=(S _s) noexcept;

    template <class S> constexpr P2  operator/ (S _s) const noexcept;
    template <class S> constexpr P2& operator/=(S _s) noexcept;

    constexpr bool operator==(const P2& _v) const;
    constexpr bool operator!=(const P2& _v) const;
    constexpr bool operator< (const P2& _v) const;
    constexpr bool operator<=(const P2& _v) const;
    constexpr bool operator> (const P2& _v) const;
    constexpr bool operator>=(const P2& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, P2& _v)       noexcept;
    friend inline    std::ostream& operator<<(std::ostream& _os, const P2& _v) noexcept;

    static constexpr P2 x_axis()   { return P2(T(1), T(0)); }
    static constexpr P2 y_axis()   { return P2(T(0), T(1)); }
    static constexpr P2 up()       { return P2(T(0), T(1)); }
    static constexpr P2 down()     { return P2(T(0), T(-1)); }
    static constexpr P2 right()    { return P2(T(1), T(0)); }
    static constexpr P2 left()     { return P2(T(-1), T(0)); }
    static constexpr P2 zero()     { return P2(T(0)); }
    static constexpr P2 one()      { return P2(T(1)); }
    static constexpr P2 infinity() { return P2(std::numeric_limits<T>::infinity()); }
    static constexpr P2 nan()      { return P2(std::numeric_limits<T>::quiet_NaN()); }

    constexpr bool has_nans()  const noexcept;
    constexpr bool is_finite() const noexcept;

    inline void set(T _s) noexcept;
    inline void clear()   noexcept;

public:
    struct { T x, y; };
    struct { T u, v; };

private:
    byte m_memory[sizeof(T) * 2];
};

using P2f = P2<flt>;
using P2i = P2<int>;


template <class T>
union P3 // Point 3D
{
    static_assert(std::is_trivially_destructible<T>::value);
public:
    constexpr P3()                          : x(0), y(0), z(0)          {}
    explicit constexpr P3(T _s)             : x(_s), y(_s), z(_s)       { sws_assert(!has_nans()); }
    explicit constexpr P3(T _x, T _y, T _z) : x(_x), y(_y), z(_z)       { sws_assert(!has_nans()); }
    constexpr P3(const P3<T> &_p)  : x(_p.x), y(_p.y), z(_p.z)          { sws_assert(!has_nans()); }
    explicit constexpr P3(const V3<T> &_v)  : x(_v.x), y(_v.y), z(_v.z) { sws_assert(!has_nans()); }

    constexpr P3<T>& operator=(const P3<T> &_p);

    template <class U> explicit constexpr operator V3<U>() const { return V3<U>(x, y, z); }
    
    constexpr T  operator[](usize _i) const noexcept { sws_assert(_i < 3); return (&x)[_i]; }
    constexpr T& operator[](usize _i) noexcept       { sws_assert(_i < 3); return (&x)[_i]; }

    constexpr P3        operator-() const noexcept;

    constexpr P3<T>  operator+ (const P3<T>& _p) const noexcept;
    constexpr P3<T>  operator+ (const V3<T>& _v) const noexcept;
    constexpr P3<T>& operator+=(const P3<T>& _p) noexcept;
    constexpr P3<T>& operator+=(const V3<T>& _v) noexcept;

    constexpr V3<T>  operator- (const P3<T>& _p) const noexcept;
    constexpr P3<T>  operator- (const V3<T>& _v) const noexcept;
    constexpr P3<T>& operator-=(const V3<T>& _v) noexcept;

    template <class S> constexpr P3  operator* (S _s) const noexcept;
    template <class S> constexpr P3& operator*=(S _s) noexcept;

    template <class S> constexpr P3  operator/ (S _s) const noexcept;
    template <class S> constexpr P3& operator/=(S _s) noexcept;

    constexpr bool operator==(const P3& _v) const;
    constexpr bool operator!=(const P3& _v) const;
    constexpr bool operator< (const P3& _v) const;
    constexpr bool operator<=(const P3& _v) const;
    constexpr bool operator> (const P3& _v) const;
    constexpr bool operator>=(const P3& _v) const;

    friend constexpr std::istream& operator>>(std::istream& _is, P3& _v)       noexcept;
    friend inline    std::ostream& operator<<(std::ostream& _os, const P3& _v) noexcept;

    static constexpr P3 x_axis()    { return P3(T(1), T(0), T(0)); }
    static constexpr P3 y_axis()    { return P3(T(0), T(1), T(0)); }
    static constexpr P3 z_axis()    { return P3(T(0), T(0), T(1)); }
    static constexpr P3 right()     { return P3(T(1), T(0), T(0)); }
    static constexpr P3 up()        { return P3(T(0), T(1), T(0)); }
    static constexpr P3 depth()     { return P3(T(0), T(0), T(1)); }
    static constexpr P3 zero()      { return P3(T(0)); }
    static constexpr P3 one()       { return P3(T(1)); }
    static constexpr P3 infinity()  { return P3(std::numeric_limits<T>::infinity()); }
    static constexpr P3 nan()       { return P3(std::numeric_limits<T>::quiet_NaN()); }

    constexpr bool has_nans()       const noexcept;
    constexpr bool is_finite()      const noexcept;

    inline void set(T _s)         noexcept;
    inline void clear()             noexcept;

    template <auto _axis> constexpr T get() const;

public:
    struct { T x, y, z; };
    struct { T r, g, b; };

private:
    u8 m_memory[sizeof(T) * 3];
};

using P3f = P3<flt>;
using P3i = P3<int>;


//template <class T>
//union N3 // Normal 3D
//{
//    static_assert(std::is_trivially_destructible<T>::value);
//public:
//    constexpr N3()                          : x(0), y(0), z(0)          {}
//    explicit constexpr N3(T _s)             : x(_s), y(_s), z(_s)       { sws_assert(!has_nans()); }
//    explicit constexpr N3(T _x, T _y, T _z) : x(_x), y(_y), z(_z)       { sws_assert(!has_nans()); }
//    explicit constexpr N3(const V3<T> &_n)  : x(_n.x), y(_n.y), z(_n.z) { sws_assert(!has_nans()); }
//    explicit constexpr N3(const P3<T> &_n)  : x(_n.x), y(_n.y), z(_n.z) { sws_assert(!has_nans()); }
//    explicit constexpr N3(const N3<T> &_n)  : x(_n.x), y(_n.y), z(_n.z) { sws_assert(!has_nans()); }
//
//    explicit constexpr N3<T>& operator=(const N3<T> &_n);
//    
//    constexpr T  operator[](usize _i) const noexcept { sws_assert(_i < 3); return (&x)[_i]; }
//    constexpr T& operator[](usize _i) noexcept       { sws_assert(_i < 3); return (&x)[_i]; }
//
//    constexpr const N3& operator+() const noexcept;
//    constexpr N3        operator-() const noexcept;
//
//    constexpr N3  operator+ (const N3& _v) const noexcept;
//    constexpr N3& operator+=(const N3& _v) noexcept;
//
//    constexpr N3  operator- (const N3& _v) const noexcept;
//    constexpr N3& operator-=(const N3& _v) noexcept;
//
//    template <class S> constexpr N3  operator* (S _s) const noexcept;
//    template <class S> constexpr N3& operator*=(S _s) noexcept;
//
//    template <class S> constexpr N3  operator/ (S _s) const noexcept;
//    template <class S> constexpr N3& operator/=(S _s) noexcept;
//
//    constexpr bool operator==(const N3& _v) const;
//    constexpr bool operator!=(const N3& _v) const;
//    constexpr bool operator< (const N3& _v) const;
//    constexpr bool operator<=(const N3& _v) const;
//    constexpr bool operator> (const N3& _v) const;
//    constexpr bool operator>=(const N3& _v) const;
//
//    friend constexpr std::istream& operator>>(std::istream& _is, V3& _v)       noexcept;
//    friend inline    std::ostream& operator<<(std::ostream& _os, const V3& _v) noexcept;
//
//    static constexpr V3 x_axis()    { return V3(T(1), T(0), T(0)); }
//    static constexpr V3 y_axis()    { return V3(T(0), T(1), T(0)); }
//    static constexpr V3 z_axis()    { return V3(T(0), T(0), T(1)); }
//    static constexpr V3 right()     { return V3(T(1), T(0), T(0)); }
//    static constexpr V3 up()        { return V3(T(0), T(1), T(0)); }
//    static constexpr V3 depth()     { return V3(T(0), T(0), T(1)); }
//    static constexpr V3 zero()      { return V3(T(0)); }
//    static constexpr V3 one()       { return V3(T(1)); }
//    static constexpr V3 infinity()  { return V3(std::numeric_limits<T>::infinity()); }
//    static constexpr V3 nan()       { return V3(std::numeric_limits<T>::quiet_NaN()); }
//
//    constexpr bool has_nans()         const noexcept;
//    constexpr bool is_finite()      const noexcept;
//    constexpr bool is_normalized()  const noexcept;
//
//    constexpr fx get_sqrlength()                                const noexcept;
//    constexpr fx get_length()                                   const noexcept;
//    constexpr V3 get_normalized()                               const noexcept;
//    constexpr V3 get_abs()                                      const noexcept;
//    constexpr V3 get_orthogonal()                               const noexcept;
//    constexpr V3 get_inverse()                                  const noexcept;
//    constexpr V3 get_floor()                                    const noexcept;
//    constexpr V3 get_ceil()                                     const noexcept;
//    constexpr V3 get_clamped(T _min, T _max)                    const noexcept;
//    constexpr fx get_shortest_angle(const V3& _axis = right())  const noexcept;
//
//    inline void set(T _val)         noexcept;
//    inline void set_length(fx _len) noexcept;
//    inline void normalize()         noexcept;
//    inline void clear()             noexcept;
//
//    template <auto _axis> constexpr T get() const;
//
//public:
//    struct { T x, y, z; };
//    struct { T r, g, b; };
//
//private:
//    u8 m_memory[sizeof(T) * 3];
//};
//
//using N3f = N3<fx>;
//using N3i = N3<ix>;



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

template<class T>
inline constexpr V2<T>& V2<T>::operator=(const V2<T>& _v)
{
    sws_assert(!has_nans());
    x = _v.x;
    y = _v.y;
    return *this;
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
constexpr V2<T> V2<T>::operator+(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return V2<T>(x + _v.x, y + _v.y);
}

template <class T>
constexpr V2<T>& V2<T>::operator+=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x += _v.x;
    y += _v.y;
    return *this;
}

template <class T>
constexpr V2<T> V2<T>::operator-(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return V2<T>(x - _v.x, y - _v.y);
}

template <class T>
constexpr V2<T>& V2<T>::operator-=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x -= _v.x;
    y -= _v.y;
    return *this;
}

template <class T>
constexpr V2<T> V2<T>::operator*(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return V2<T>(x * _v.x, y * _v.y);
}

template <class T>
constexpr V2<T>& V2<T>::operator*=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x *= _v.x;
    y *= _v.y;
    return *this;
}

template <class T>
template <class S>
constexpr V2<T> V2<T>::operator*(S _s) const noexcept
{
    sws_assert(!std::isnan(_s));
    return V2<T>(x * _s, y * _s);
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
constexpr V2<T> V2<T>::operator/(S _s) const noexcept
{
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    return V2<T>(x * inv_s, y * inv_s);
}

template <class T>
template <class S>
constexpr V2<T>& V2<T>::operator/=(S _s) noexcept
{
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    return *this;
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
constexpr bool V2<T>::has_nans() const noexcept
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
    const flt sum = x + y;
    return math::are_almost_equal(sum, 1.f);
}

template <class T>
constexpr flt V2<T>::get_sqrlength() const noexcept
{
    return x * x + y * y;
}

template <class T>
constexpr flt V2<T>::get_length() const noexcept
{
    return math::sqrt(get_sqrlength());
}

template <class T>
constexpr V2<T> V2<T>::get_normalized() const noexcept
{
    const flt inv_len = math::inv(get_length());
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
constexpr flt V2<T>::get_angle(const V2<T>& _axis) const noexcept
{
    const flt x = math::dot(_axis, *this);
    const flt y = math::dot(_axis.get_orthogonal(), *this);
    const flt angle = math::atan2(y, x);
    return angle;
}

template <class T>
constexpr flt V2<T>::get_shortest_angle(const V2<T>& _axis) const noexcept
{
    const flt angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
    sws_assert(angle >= 0.f);
    sws_assert(angle < math::Pi2f);
    return angle;
}

template<class T>
inline void V2<T>::set(T _s) noexcept
{
    x = y = _s;
}

template <class T>
inline void V2<T>::set_length(flt _len) noexcept
{
    normalize();
    *this *= _len;
}

template <class T>
inline void V2<T>::normalize() noexcept
{
    *this = get_normalized();
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

// V3: Vector3D

template<class T>
inline constexpr V3<T>& V3<T>::operator=(const V3<T>& _v)
{
    sws_assert(!has_nans());
    x = _v.x;
    y = _v.y;
    z = _v.z;
    return *this;
}

template <class T>
constexpr const V3<T>& V3<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
constexpr V3<T> V3<T>::operator-() const noexcept
{
    return V3<T>(-x, -y, -z);
}

template <class T>
constexpr V3<T> V3<T>::operator+(const V3<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return V3<T>(x + _v.x, y + _v.y, z + _v.z);
}

template <class T>
constexpr V3<T>& V3<T>::operator+=(const V3<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x += _v.x;
    y += _v.y;
    z += _v.z;
    return *this;
}

template <class T>
constexpr V3<T> V3<T>::operator-(const V3<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return V3<T>(x - _v.x, y - _v.y, z - _v.z);
}

template <class T>
constexpr V3<T>& V3<T>::operator-=(const V3<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x -= _v.x;
    y -= _v.y;
    z -= _v.z;
    return *this;
}

template <class T>
constexpr V3<T> V3<T>::operator*(const V3<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return V3<T>(x * _v.x, y * _v.y, z * _v.z);
}

template <class T>
constexpr V3<T>& V3<T>::operator*=(const V3<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x *= _v.x;
    y *= _v.y;
    z *= _v.z;
    return *this;
}

template <class T>
template <class S>
constexpr V3<T> V3<T>::operator*(S _s) const noexcept
{
    sws_assert(!math::is_nan(_s));
    return V3<T>(x * _s, y * _s, z * _s);
}

template <class T>
template <class S>
constexpr V3<T>& V3<T>::operator*=(S _s) noexcept
{
    sws_assert(!math::is_nan(_s));
    x *= _s;
    y *= _s;
    z *= _s;
    return *this;
}

template <class T>
template <class S>
constexpr V3<T> V3<T>::operator/(S _s) const noexcept
{
    sws_assert(!math::is_nan(_s));
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    return V3<T>(x * inv_s, y * inv_s, z * inv_s);
}

template <class T>
template <class S>
constexpr V3<T>& V3<T>::operator/=(S _s) noexcept
{
    sws_assert(!math::is_nan(_s));
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
    return *this;
}

template <class T>
constexpr bool V3<T>::operator==(const V3& _v) const
{
    return x == _v.x && y == _v.y && z == _v.z;
}

template <class T>
constexpr bool V3<T>::operator!=(const V3& _v) const
{
    return x != _v.x || y != _v.y || z != _v.z;
}

template <class T>
constexpr bool V3<T>::operator<(const V3& _v) const
{
    return x < _v.x && y < _v.y && z < _v.z;
}

template <class T>
constexpr bool V3<T>::operator<=(const V3& _v) const
{
    return x <= _v.x && y <= _v.y && z <= _v.z;
}

template <class T>
constexpr bool V3<T>::operator>(const V3& _v) const
{
    return x > _v.x && y > _v.y && z > _v.z;
}

template <class T>
constexpr bool V3<T>::operator>=(const V3& _v) const
{
    return x >= _v.x && y >= _v.y && z >= _v.z;
}

template <class T>
constexpr std::istream& operator>>(std::istream& _is, V3<T>& _v) noexcept
{
    _is >> _v.x >> _v.y >> _v.z;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const V3<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y << ' ' << _v.z;
    return _os;
}

template <class T>
constexpr bool V3<T>::has_nans() const noexcept
{
    return math::is_nan(x) || math::is_nan(y) || math::is_nan(z);
}

template <class T>
constexpr bool V3<T>::is_finite() const noexcept
{
    return math::is_finite(x) && math::is_finite(y) && math::is_finite(z);
}

template <class T>
constexpr bool V3<T>::is_normalized() const noexcept
{
    const flt sum = x + y + z;
    return math::are_almost_equal(sum, 1.f);
}

template <class T>
constexpr flt V3<T>::get_sqrlength() const noexcept
{
    return x * x + y * y + z * z;
}

template <class T>
constexpr flt V3<T>::get_length() const noexcept
{
    return math::sqrt(get_sqrlength());
}

template <class T>
constexpr V3<T> V3<T>::get_normalized() const noexcept
{
    const flt inv_len = math::inv(get_length());
    return V3<T>(x * inv_len, y * inv_len, z * inv_len);
}

template <class T>
constexpr V3<T> V3<T>::get_abs() const noexcept
{
    return V3<T>(math::abs(x), math::abs(y), math::abs(z));
}

template <class T>
constexpr V3<T> V3<T>::get_orthogonal() const noexcept
{
    const flt len = get_length();
    if (math::are_almost_equal(z, T(0)))
        return up() * len;

    const T out_x = y;
    const T out_y = z;
    const T out_z = -(x * out_x + y * out_y) / z;
    V3<T> out(out_x, out_y, out_z);
    out.set_length(len);
    return out;
}

template <class T>
constexpr V3<T> V3<T>::get_inverse() const noexcept
{
    return V3<T>(math::inv(x), math::inv(y), math::inv(z));
}

template <class T>
constexpr V3<T> V3<T>::get_floor() const noexcept
{
    return V3<T>(math::floor(x), math::floor(y), math::floor(z));
}

template <class T>
constexpr V3<T> V3<T>::get_ceil() const noexcept
{
    return V3<T>(math::ceil(x), math::ceil(y), math::ceil(z));
}

template <class T>
constexpr V3<T> V3<T>::get_clamped(T _min, T _max) const noexcept
{
    return V3<T>(math::clamp(x, _min, _max), math::clamp(y, _min, _max), math::clamp(z, _min, _max));
}

template <class T>
constexpr flt V3<T>::get_shortest_angle(const V3<T>& _axis) const noexcept
{
    const flt angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
    sws_assert(angle >= 0.f);
    sws_assert(angle < math::Pi2f);
    return angle;
}

template<class T>
inline void V3<T>::set(T _s) noexcept
{
    x = y = z = _s;
}

template <class T>
inline void V3<T>::set_length(flt _len) noexcept
{
    normalize();
    *this *= _len;
}

template <class T>
inline void V3<T>::normalize() noexcept
{
    *this = get_normalized();
}

template <class T>
inline void V3<T>::clear() noexcept
{
    x = y = z = T(0);
}

template <class T>
template<auto _axis>
constexpr T V3<T>::get() const
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
inline V3<T> operator*(T _s, const V3<T>& _v)
{
    return _v.operator*(_s);
}


namespace math
{
    template <class T>
    constexpr T sqrdist(const V3<T>& _a, const V3<T>& _b) noexcept
    {
        return (_b - _a).get_sqrlength();
    }

    template <class T>
    constexpr T dist(const V3<T>& _a, const V3<T>& _b) noexcept
    {
        return (_b - _a).get_length();
    }

    template <class T>
    constexpr T dot(const V3<T>& _a, const V3<T>& _b) noexcept
    {
        return _a.x*_b.x + _a.y*_b.y + _a.z*_b.z;
    }

    template <class T>
    constexpr V3<T> cross(const V3<T>& _a, const V3<T>& _b) noexcept
    {
        return V3<T>(
            _a.y*_b.z - _a.z*_b.y,
            _a.z*_b.x - _a.x*_b.z,
            _a.x*_b.y - _a.y*_b.x
        );
    }
}

template<class T>
inline constexpr P2<T>& P2<T>::operator=(const P2<T>& _p)
{
    sws_assert(!has_nans());
    x = _p.x;
    y = _p.y;
    return *this;
}

template<class T>
inline constexpr P2<T> P2<T>::operator-() const noexcept
{
    return P2<T>(-x, -y);
}

template<class T>
inline constexpr P2<T> P2<T>::operator+(const P2 & _p) const noexcept
{
    sws_assert(_p.has_nans());
    return P2<T>(x + _p.x, y + _p.y);
}

template<class T>
inline constexpr P2<T> P2<T>::operator+(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return P2<T>(x + _v.x, y + _v.y);
}

template<class T>
inline constexpr P2<T>& P2<T>::operator+=(const P2<T>& _p) noexcept
{
    sws_assert(!_p.has_nans());
    x += _p.x;
    y += _p.y;
    return *this;
}

template<class T>
inline constexpr P2<T>& P2<T>::operator+=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x += _v.x;
    y += _v.y;
    return *this;
}

template<class T>
inline constexpr V2<T> P2<T>::operator-(const P2<T>& _p) const noexcept
{
    sws_assert(!_p.has_nans());
    return P2<T>(x - _p.x, y - _p.y);
}

template<class T>
inline constexpr P2<T> P2<T>::operator-(const V2<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return P2<T>(x - _v.x, y - _v.y);
}

template<class T>
inline constexpr P2<T>& P2<T>::operator-=(const V2<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x -= _v.x;
    y -= _v.y;
    return *this;
}

template<class T>
template<class S>
inline constexpr P2<T> P2<T>::operator*(S _s) const noexcept
{
    sws_assert(!math::is_nan(_s));
    return P2<T>(_s * x, _s * y);
}

template<class T>
template<class S>
inline constexpr P2<T> & P2<T>::operator*=(S _s) noexcept
{
    sws_assert(!math::is_nan(_s));
    x *= _s;
    y *= _s;
    return *this;
}

template<class T>
template<class S>
inline constexpr P2<T> P2<T>::operator/(S _s) const noexcept
{
    sws_assert(!math::is_nan(_s));
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    return P2<T>(x * inv_s, y * inv_s);
}

template <class T>
template <class S>
constexpr P2<T>& P2<T>::operator/=(S _s) noexcept
{
    sws_assert(!math::is_nan(_s));
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    return *this;
}

template<class T>
inline constexpr bool P2<T>::operator==(const P2& _v) const
{
    return x == _v.x && y == _v.y;
}

template<class T>
inline constexpr bool P2<T>::operator!=(const P2& _v) const
{
    return x != _v.x || y != _v.y;
}

template<class T>
inline constexpr bool P2<T>::operator<(const P2 & _v) const
{
    return x < _v.x && y < _v.y;
}

template<class T>
inline constexpr bool P2<T>::operator<=(const P2 & _v) const
{
    return x <= _v.x && y <= _v.y;
}

template<class T>
inline constexpr bool P2<T>::operator>(const P2& _v) const
{
    return x > _v.x && y > _v.y;
}

template<class T>
inline constexpr bool P2<T>::operator>=(const P2& _v) const
{
    return x >= _v.x && y >= _v.y;
}

template <class T>
inline constexpr std::istream& operator>>(std::istream& _is, P2<T>& _v) noexcept
{
    _is >> _v.x >> _v.y;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const P2<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y;
    return _os;
}

template<class T>
inline constexpr bool P2<T>::has_nans() const noexcept
{
    return math::is_nan(x) || math::is_nan(y);
}

template<class T>
inline constexpr bool P2<T>::is_finite() const noexcept
{
    return !has_nans();
}

template<class T>
inline void P2<T>::set(T _s) noexcept
{
    x = y = _s;
}

template<class T>
inline void P2<T>::clear() noexcept
{
    x = y = T(0);
}


// Point 3D

template<class T>
inline constexpr P3<T>& P3<T>::operator=(const P3<T>& _p)
{
    sws_assert(!has_nans());
    x = _p.x;
    y = _p.y;
    z = _p.z;
    return *this;
}

template<class T>
inline constexpr P3<T> P3<T>::operator-() const noexcept
{
    return P3<T>(-x, -y, -z);
}

template<class T>
inline constexpr P3<T> P3<T>::operator+(const P3& _p) const noexcept
{
    sws_assert(_p.has_nans());
    return P3<T>(x + _p.x, y + _p.y, z + _p.z);
}

template<class T>
inline constexpr P3<T> P3<T>::operator+(const V3<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return P3<T>(x + _v.x, y + _v.y, z + _v.z);
}

template<class T>
inline constexpr P3<T>& P3<T>::operator+=(const P3<T>& _p) noexcept
{
    sws_assert(!_p.has_nans());
    x += _p.x;
    y += _p.y;
    z += _p.z;
    return *this;
}

template<class T>
inline constexpr P3<T>& P3<T>::operator+=(const V3<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x += _v.x;
    y += _v.y;
    z += _v.z;
    return *this;
}

template<class T>
inline constexpr V3<T> P3<T>::operator-(const P3<T>& _p) const noexcept
{
    sws_assert(!_p.has_nans());
    return P2<T>(x - _p.x, y - _p.y, z - _p.z);
}

template<class T>
inline constexpr P3<T> P3<T>::operator-(const V3<T>& _v) const noexcept
{
    sws_assert(!_v.has_nans());
    return P2<T>(x - _v.x, y - _v.y, z - _v.z);
}

template<class T>
inline constexpr P3<T>& P3<T>::operator-=(const V3<T>& _v) noexcept
{
    sws_assert(!_v.has_nans());
    x -= _v.x;
    y -= _v.y;
    z -= _v.z;
    return *this;
}

template<class T>
template<class S>
inline constexpr P3<T> P3<T>::operator*(S _s) const noexcept
{
    sws_assert(!math::is_nan(_s));
    return P2<T>(_s * x, _s * y, _s * z);
}

template<class T>
template<class S>
inline constexpr P3<T> & P3<T>::operator*=(S _s) noexcept
{
    sws_assert(!math::is_nan(_s));
    x *= _s;
    y *= _s;
    z *= _s;
    return *this;
}

template<class T>
template<class S>
inline constexpr P3<T> P3<T>::operator/(S _s) const noexcept
{
    sws_assert(!math::is_nan(_s));
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    return P2<T>(x * inv_s, y * inv_s, z * inv_s);
}

template <class T>
template <class S>
constexpr P3<T>& P3<T>::operator/=(S _s) noexcept
{
    sws_assert(!math::is_nan(_s));
    sws_verify(_s != T(0));
    const flt inv_s = math::inv(_s);
    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
    return *this;
}

template<class T>
inline constexpr bool P3<T>::operator==(const P3& _v) const
{
    return x == _v.x && y == _v.y && z == _v.z;
}

template<class T>
inline constexpr bool P3<T>::operator!=(const P3& _v) const
{
    return x != _v.x || y != _v.y || z != _v.z;
}

template<class T>
inline constexpr bool P3<T>::operator<(const P3& _v) const
{
    return x < _v.x && y < _v.y && z < _v.z;
}

template<class T>
inline constexpr bool P3<T>::operator<=(const P3& _v) const
{
    return x <= _v.x && y <= _v.y && z <= _v.z;
}

template<class T>
inline constexpr bool P3<T>::operator>(const P3& _v) const
{
    return x > _v.x && y > _v.y && z > _v.z;
}

template<class T>
inline constexpr bool P3<T>::operator>=(const P3& _v) const
{
    return x >= _v.x && y >= _v.y && z >= _v.z;
}

template <class T>
inline constexpr std::istream& operator>>(std::istream& _is, P3<T>& _v) noexcept
{
    _is >> _v.x >> _v.y >> _v.z;
    return _is;
}

template <class T>
inline std::ostream& operator<<(std::ostream& _os, const P3<T>& _v) noexcept
{
    _os << _v.x << ' ' << _v.y << ' ' << _v.z;
    return _os;
}

template<class T>
inline constexpr bool P3<T>::has_nans() const noexcept
{
    return math::is_nan(x) || math::is_nan(y) || math::is_nan(z);
}

template<class T>
inline constexpr bool P3<T>::is_finite() const noexcept
{
    return !has_nans();
}

template<class T>
inline void P3<T>::set(T _s) noexcept
{
    x = y = z = _s;
}

template<class T>
inline void P3<T>::clear() noexcept
{
    x = y = z = T(0);
}