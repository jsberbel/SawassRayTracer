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

    template <typename Type>
    constexpr Type ToRadians( Type degrees )
    {
        return( degrees * ( Math::PI<F32> / 180.f ) );
    }

    template <typename Type>
    constexpr Type ToDegrees( Type radians )
    {
        return( radians * ( 180.f / Math::PI<F32> ) );
    }

    template<typename T>
    using return_t = typename std::conditional<std::is_integral<T>::value, double, T>::type;

    #ifndef GCEM_PI
    #define GCEM_PI 3.14159265358979323846L
    #endif

    template<class T>
    using GCLIM = std::numeric_limits<T>;

    template<typename T>
    constexpr
        T
        Abs( const T x )
    {
        return( x < T( 0 ) ? -x : x );
    }

        template<typename T>
    constexpr
    T
    tan_cf_recur(const T xx, const int depth, const int max_depth)
    {
        return( depth < max_depth ? \
                // if
                    T(2*depth - 1) - xx/tan_cf_recur(xx,depth+1,max_depth) :
                // else
                    T(2*depth - 1) );
    }

    template<typename T>
    constexpr
    T
    tan_cf_main(const T x)
    {
        return( x > T(1) ? \
                // if
                    x/tan_cf_recur(x*x,1,35) :
                // else
                    x/tan_cf_recur(x*x,1,25) );
    }

    template<typename T>
    constexpr
    T
    tan_int(const T x)
    {   // tan(x) = tan(x + pi)
        return( x > T(GCEM_PI) ? \
                // if
                    tan_int( x - T(GCEM_PI) * int(x/T(GCEM_PI)) ) :
                // else 
                    tan_cf_main(x) );
    }

    template<typename T>
    constexpr
    T
    tan_check(const T x)
    {
        return( // indistinguishable from zero 
                GCLIM<T>::epsilon() > Abs(x) ? \
                    T(0) :
                // else
                    x < T(0) ? \
                        - tan_int(-x) : 
                          tan_int( x) );
    }

    template<typename T>
    constexpr
    return_t<T>
    Tan(const T x)
    {
        return tan_check<return_t<T>>(x);
    }
}