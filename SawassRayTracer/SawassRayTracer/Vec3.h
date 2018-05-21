#pragma once

#include <cmath>
#include <iostream>
#include <xutility>

#include "Types.h"
#include "Assert.h"

template <typename Type>
class Vec3
{
public:
    constexpr Vec3() noexcept;
    constexpr Vec3( Type t ) noexcept;
    constexpr Vec3( Type x, Type y, Type z ) noexcept;

    constexpr Vec3( const Vec3& v ) noexcept;
    constexpr Vec3( Vec3&& v ) noexcept;

    constexpr Vec3& operator=(const Vec3& v) noexcept;
    constexpr Vec3& operator=(Vec3&& v) noexcept;

    constexpr const Vec3& operator+() const noexcept;
    constexpr Vec3 operator-() const noexcept;
    inline Type operator[]( Size i ) const noexcept;
    inline Type& operator[]( Size i ) noexcept;

    inline Vec3& operator+=( const Vec3& v ) noexcept;
    inline Vec3& operator-=( const Vec3& v ) noexcept;
    inline Vec3& operator*=( const Vec3& v ) noexcept;
    inline Vec3& operator/=( const Vec3& v ) noexcept;
    inline Vec3& operator*=( const Type t ) noexcept;
    inline Vec3& operator/=( const Type t ) noexcept;

    friend inline std::istream& operator>>( std::istream &is, Vec3& v ) noexcept;
    friend inline std::ostream& operator<<( std::ostream &os, const Vec3& v ) noexcept;

    inline Type GetLength() const noexcept;
    inline Type GetSquaredLength() const noexcept;
    inline Vec3 GetUnit() const noexcept;
    inline void MakeUnit() noexcept;

    static Type Dot( const Vec3& lhs, const Vec3& rhs ) noexcept;
    static Vec3 Cross( const Vec3& lhs, const Vec3& rhs ) noexcept;

public:
    static const Vec3 Unit;

    union
    {
        struct { Type X, Y, Z; };
        struct { Type R, G, B; };
        struct { Type S, T, P; };
    };
};

template <typename Type>
const Vec3<Type> Vec3<Type>::Unit( 1.f );

template <typename Type>
constexpr Vec3<Type>::Vec3() noexcept = default;

template <typename Type>
constexpr Vec3<Type>::Vec3( Type t ) noexcept
    : X(t), Y(t), Z(t)
{
}

template <typename Type>
constexpr Vec3<Type>::Vec3( Type x, Type y, Type z ) noexcept
    : X(x), Y(y), Z(z)
{
}

template <typename Type>
constexpr Vec3<Type>::Vec3( const Vec3& v ) noexcept
    : X(v.X), Y(v.Y), Z(v.Z)
{
}

template <typename Type>
constexpr Vec3<Type>::Vec3( Vec3&& v ) noexcept
    : X( std::move(v.X) ), Y( std::move(v.Y) ), Z( std::move(v.Z) )
{
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator=( const Vec3& v ) noexcept
{
    if (this != &v)
    {
        X = v.X;
        Y = v.Y;
        Z = v.Z;
    }
    return *this;
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator=( Vec3<Type>&& v ) noexcept
{
    if (this != &v)
    {
        X = std::exchange( v.X, 0.f );
        Y = std::exchange( v.Y, 0.f );
        Z = std::exchange( v.Z, 0.f );
    }
    return *this;
}

template <typename Type>
constexpr const Vec3<Type>& Vec3<Type>::operator+() const noexcept
{
    return *this;
}

template <typename Type>
constexpr Vec3<Type> Vec3<Type>::operator-() const noexcept
{
    return Vec3<Type>( -X, -Y, -Z );
}

template <typename Type>
inline Type Vec3<Type>::operator[](Size i) const noexcept
{
    Assert( i < 3 );
    return (&X)[i];
}

template <typename Type>
inline Type& Vec3<Type>::operator[](Size i) noexcept
{
    Assert(i < 3);
    return (&X)[i];
}

template <typename Type>
inline Vec3<Type>& Vec3<Type>::operator+=( const Vec3<Type>& v ) noexcept
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    return *this;
}

template <typename Type>
inline Vec3<Type>& Vec3<Type>::operator-=( const Vec3<Type>& v ) noexcept
{
    X -= v.X;
    Y -= v.Y;
    Z -= v.Z;
    return *this;
}

template <typename Type>
inline Vec3<Type>& Vec3<Type>::operator*=( const Vec3<Type>& v ) noexcept
{
    X *= v.X;
    Y *= v.Y;
    Z *= v.Z;
    return *this;
}

template <typename Type>
inline Vec3<Type>& Vec3<Type>::operator/=( const Vec3<Type>& v ) noexcept
{
    X /= v.X;
    Y /= v.Y;
    Z /= v.Z;
    return *this;
}

template <typename Type>
inline Vec3<Type>& Vec3<Type>::operator*=( const Type t ) noexcept
{
    X *= t;
    Y *= t;
    Z *= t;
    return *this;
}

template <typename Type>
inline Vec3<Type>& Vec3<Type>::operator/=( const Type t ) noexcept
{
    const float k = 1.f / t;
    X *= k;
    Y *= k;
    Z *= k;
    return *this;
}

template <typename Type>
inline std::istream& operator>>(std::istream& is, Vec3<Type>& v) noexcept
{
    is >> v.X >> v.Y >> v.Z;
    return is;
}

template <typename Type>
inline std::ostream& operator<<(std::ostream& os, const Vec3<Type>& v) noexcept
{
    os << v.X << " " << v.Y << " " << v.Z;
    return os;
}

template <typename Type>
inline Type Vec3<Type>::GetLength() const noexcept
{
    return std::sqrt( GetSquaredLength() );
}

template <typename Type>
inline Type Vec3<Type>::GetSquaredLength() const noexcept
{
    return X * X + Y * Y + Z * Z;
}

template <typename Type>
inline Vec3<Type> Vec3<Type>::GetUnit() const noexcept
{
    const Type k = 1.f / GetLength();
    return Vec3<Type>( X * k, Y * k, Z * k );
}

template <typename Type>
inline void Vec3<Type>::MakeUnit() noexcept
{
    const Type k = 1.f / GetLength();
    X *= k;
    Y *= k;
    Z *= k;
}

template <typename Type>
inline Vec3<Type> operator+( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z );
}

template <typename Type>
inline Vec3<Type> operator-( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z );
}

template <typename Type>
inline Vec3<Type> operator*( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z );
}

template <typename Type>
inline Vec3<Type> operator/( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z );
}

template <typename Type>
inline Vec3<Type> operator*( Type t, const Vec3<Type>& v ) noexcept
{
    return Vec3<Type>( t * v.X, t * v.Y, t * v.Z );
}

template <typename Type>
inline Vec3<Type> operator*( const Vec3<Type>& v, Type t ) noexcept
{
    return Vec3<Type>( v.X * t, v.Y * t, v.Z * t );
}

template <typename Type>
inline Vec3<Type> operator/( const Vec3<Type>& v, Type t ) noexcept
{
    return Vec3<Type>( v.X / t, v.Y / t, v.Z / t );
}

template <typename Type>
Type Vec3<Type>::Dot( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

template <typename Type>
Vec3<Type> Vec3<Type>::Cross( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.Y * rhs.Z - lhs.Z * rhs.Y ,
                       lhs.Z * rhs.X - lhs.X * rhs.Z ,
                       lhs.X * rhs.Y - lhs.Y * rhs.X );
}

using BVec3   = Vec3<Byte>;

using U8Vec3  = Vec3<U8>;
using U16Vec3 = Vec3<U16>;
using U32Vec3 = Vec3<U32>;
using U64Vec3 = Vec3<U64>;

using S8Vec3  = Vec3<U8>;
using S16Vec3 = Vec3<U16>;
using S32Vec3 = Vec3<U32>;
using S64Vec3 = Vec3<U64>;

using FVec3   = Vec3<F32>;
using DVec3   = Vec3<F64>;