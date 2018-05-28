#pragma once

#include <Core/Vec3.h>
#include <Core/Assert.h>

namespace Math
{
    template <class Type> 
    constexpr F32 PI = Type(3.141592653589793238462643383279502884L);

    template <typename Type>
    constexpr Type Dot(const Vec3<Type>& lhs, const Vec3<Type>& rhs) noexcept
    {
        return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
    }

    template <typename Type>
    constexpr Vec3<Type> Cross(const Vec3<Type>& lhs, const Vec3<Type>& rhs) noexcept
    {
        return Vec3<Type>(lhs.Y * rhs.Z - lhs.Z * rhs.Y,
                          lhs.Z * rhs.X - lhs.X * rhs.Z,
                          lhs.X * rhs.Y - lhs.Y * rhs.X);
    }

    template <class Type>
    constexpr Type Blend( const Type& start, const Type& end, F32 t )
    {
        return (1.f - t) * start + t * end;
    }

    template<class Type, class Compare>
    constexpr Type Clamp( const Type& v, const Type& low, const Type& hi, Compare comp )
    {
        Assert( !comp( hi, low ) );
        return comp( v, low ) ? low : comp( hi, v ) ? hi : v;
    }

    template<class Type>
    constexpr Type Clamp( const Type& v, const Type& low, const Type& hi )
    {
        return Clamp( v, low, hi, std::less<>() );
    }

    template <typename Type>
    constexpr Type Pow( Type num, U32 pow )
    {
        return ( pow >= sizeof( U32 ) * 8 ) ? 0 :
                 pow == 0 ? 1 : num * Pow( num, pow - 1 );
    }
}