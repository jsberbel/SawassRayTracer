#pragma once

#include <cmath>
#include <iostream>
#include <xutility>

#include <Core/Types.h>
#include <Core/Assert.h>

template <typename Type>
class Vec3
{
public:
    constexpr Vec3() noexcept;
    constexpr Vec3( Type t ) noexcept;
    constexpr Vec3( Type x, Type y, Type z ) noexcept;

    constexpr Vec3( const Vec3& v ) noexcept;
    constexpr Vec3( Vec3&& v ) noexcept;

    constexpr Vec3& operator=( const Vec3& v ) noexcept;
    constexpr Vec3& operator=( Vec3&& v ) noexcept;

    constexpr const Vec3& operator+() const noexcept;
    constexpr Vec3 operator-() const noexcept;
    constexpr Type operator[]( Size i ) const noexcept;
    constexpr Type& operator[]( Size i ) noexcept;

    constexpr Vec3& operator+=( const Vec3& v ) noexcept;
    constexpr Vec3& operator-=( const Vec3& v ) noexcept;
    constexpr Vec3& operator*=( const Vec3& v ) noexcept;
    constexpr Vec3& operator/=( const Vec3& v ) noexcept;
    constexpr Vec3& operator*=( const Type t ) noexcept;
    constexpr Vec3& operator/=( const Type t ) noexcept;

    friend constexpr std::istream& operator>>( std::istream &is, Vec3& v ) noexcept;
    friend constexpr std::ostream& operator<<( std::ostream &os, const Vec3& v ) noexcept;

    constexpr Type Length() const noexcept;
    constexpr Type SquaredLength() const noexcept;
    constexpr Vec3 Normalize() const noexcept;

public:
    union
    {
        struct { Type X, Y, Z; };
        struct { Type R, G, B; };
        struct { Type S, T, P; };
    };
};

template <typename Type>
constexpr Vec3<Type>::Vec3() noexcept = default;

template <typename Type>
constexpr Vec3<Type>::Vec3( Type t ) noexcept
    : X( t ), Y( t ), Z( t )
{
}

template <typename Type>
constexpr Vec3<Type>::Vec3( Type x, Type y, Type z ) noexcept
    : X( x ), Y( y ), Z( z )
{
}

template <typename Type>
constexpr Vec3<Type>::Vec3( const Vec3& v ) noexcept
    : X( v.X ), Y( v.Y ), Z( v.Z )
{
}

template <typename Type>
constexpr Vec3<Type>::Vec3( Vec3&& v ) noexcept
    : X( std::move( v.X ) ), Y( std::move( v.Y ) ), Z( std::move( v.Z ) )
{
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator=( const Vec3& v ) noexcept
{
    if( this != &v )
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
    if( this != &v )
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
constexpr Type Vec3<Type>::operator[]( Size i ) const noexcept
{
    Assert( i < 3 );
    return ( &X )[i];
}

template <typename Type>
constexpr Type& Vec3<Type>::operator[]( Size i ) noexcept
{
    Assert( i < 3 );
    return ( &X )[i];
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator+=( const Vec3<Type>& v ) noexcept
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    return *this;
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator-=( const Vec3<Type>& v ) noexcept
{
    X -= v.X;
    Y -= v.Y;
    Z -= v.Z;
    return *this;
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator*=( const Vec3<Type>& v ) noexcept
{
    X *= v.X;
    Y *= v.Y;
    Z *= v.Z;
    return *this;
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator/=( const Vec3<Type>& v ) noexcept
{
    X /= v.X;
    Y /= v.Y;
    Z /= v.Z;
    return *this;
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator*=( const Type t ) noexcept
{
    X *= t;
    Y *= t;
    Z *= t;
    return *this;
}

template <typename Type>
constexpr Vec3<Type>& Vec3<Type>::operator/=( const Type t ) noexcept
{
    const float k = 1.f / t;
    X *= k;
    Y *= k;
    Z *= k;
    return *this;
}

template <typename Type>
constexpr std::istream& operator>>( std::istream& is, Vec3<Type>& v ) noexcept
{
    is >> v.X >> v.Y >> v.Z;
    return is;
}

template <typename Type>
inline std::ostream& operator<<( std::ostream& os, const Vec3<Type>& v ) noexcept
{
    os << v.X << " " << v.Y << " " << v.Z;
    return os;
}

template <typename Type>
constexpr Type Vec3<Type>::Length() const noexcept
{
    return std::sqrt( SquaredLength() );
}

template <typename Type>
constexpr Type Vec3<Type>::SquaredLength() const noexcept
{
    return X * X + Y * Y + Z * Z;
}

template <typename Type>
constexpr Vec3<Type> Vec3<Type>::Normalize() const noexcept
{
    const Type k = 1.f / Length();
    return Vec3<Type>( X * k, Y * k, Z * k );
}

template <typename Type>
constexpr Vec3<Type> operator+( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z );
}

template <typename Type>
constexpr Vec3<Type> operator-( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z );
}

template <typename Type>
constexpr Vec3<Type> operator*( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z );
}

template <typename Type>
constexpr Vec3<Type> operator/( const Vec3<Type>& lhs, const Vec3<Type>& rhs ) noexcept
{
    return Vec3<Type>( lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z );
}

template <typename Type>
constexpr Vec3<Type> operator*( const Type& t, const Vec3<Type>& v ) noexcept
{
    return Vec3<Type>( t * v.X, t * v.Y, t * v.Z );
}

template <typename Type>
constexpr Vec3<Type> operator*( const Vec3<Type>& v, Type t ) noexcept
{
    return Vec3<Type>( v.X * t, v.Y * t, v.Z * t );
}

template <typename Type>
constexpr Vec3<Type> operator/( const Vec3<Type>& v, Type t ) noexcept
{
    return Vec3<Type>( v.X / t, v.Y / t, v.Z / t );
}

using BVec3 = Vec3<bool>;

using UVec3 = Vec3<U32>;
using SVec3 = Vec3<S32>;

using FVec3 = Vec3<F32>;
using DVec3 = Vec3<F64>;

using RGB = Vec3<Byte>;