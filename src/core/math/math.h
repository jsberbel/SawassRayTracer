// ======================================================================
// File: math.h
// Revision: 1.0
// Creation: 05/22/2018 - jsberbel
// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
// ======================================================================

#pragma once

#include "core/assert.h"
#include "core/types.h"
#include "core/traits.h"

#include <cmath>
#include <xmmintrin.h>
#include <emmintrin.h>

namespace math
{
    /**
     * Constant that represents the perimeter of the unit semicircle (r = 1).
    */
    template <class T>
    inline constexpr T Pi = T(3.141592653589793238462643383279502884L);
    
    /**
     * Constant that represents the perimeter of the unit circle (r = 1).
     */
    template <class T>
    inline constexpr T Pi2 = Pi<T> * T(2);

    /**
     * Returns whether or not _x is NaN (not a number).
     */
    template <typename T>
    constexpr T is_nan(const T& _x)
    {
        return std::isnan(_x);
    }
    
    /**
     * Returns whether or not _x has finite value (neither NaN nor Infinite).
     */
    template <typename T>
    constexpr T is_finite(const T& _x)
    {
        return std::isfinite(_x);
    }

    /**
     * Returns the sign bit if _x is signed.
     */
    template <typename T>
    constexpr T sign(const T& _x)
    {
        static_assert(std::is_signed<T>::value);
        return _x >= T(0) ? T(1) : T(-1);
    }
    
    /**
     * Returns whether or not _x is negative.
     */
    template <typename T>
    constexpr b32 is_negative(const T& _x)
    {
        static_assert(std::is_signed<T>::value);
        return _x < T(0);
    }
    
    /**
     * Returns the absolute value of _x.
     */
    template <typename T>
    constexpr T abs(T _x)
    {
        static_assert(std::is_signed<T>::value);
        return std::abs(_x);
    }
    
    /**
     * Returns the inverse of _x.
     */
    template <typename T>
    constexpr T inv(T _x)
    {
        return T(1) / _x;
    }
    
    /**
     * Returns the largest integer value not greater than _x.
     */
    template <typename T>
    constexpr T floor(T _x)
    {
        return std::floor(_x);
    }
    
    /**
     * Returns the smallest integer value not less than _x.
     */
    template <typename T>
    constexpr T ceil(T _x)
    {
        return std::ceil(_x);
    }

    /**
     * Returns _x squared (i.e. _x * _x).
     */
    template <typename T>
    constexpr T sqr(T _x)
    {
        return _x * _x;
    }
    
    /**
     * Returns the square root of _x.
     */
    template <typename T>
    constexpr T sqrt(T _x)
    {
        return std::sqrt(_x);
    }
    
    /**
     * Returns the remainder after division of _x by _m.
     * @param _x Dividend.
     * @param _m Divisor.
     */
    template <typename T, ENABLE_IF(IS_REAL(T))>
    constexpr T fmod(T _x, T _m)
    {
        return std::fmod(_x, _m);
    }
    
    /**
     * Returns _b raised to the power of _e.
     * @param _b Base.
     * @param _e Exponent.
     */
    template <typename T>
    constexpr T pow(T _b, u32 _e)
    {
        return std::pow(_b, _e);
    }
    
    /**
     * Returns ref to _min if v is less than _min, ref to _max if _max is less than _v, otherwise ref to _v.
     * @param _x      Value to clamp.
     * @param _min    Low boundary.
     * @param _max    High boundary.
     * @param _cmp_fn Comparison function object that returns ​true if the first arg is less than the second.
     */
    template <class T, class C>
    constexpr const T& clamp(const T& _x, const T& _min, const T& _max, C _cmp_fn)
    {
        sws_assert( !_cmp_fn(_max, _min) );
        return _cmp_fn(_x, _min) ? _min : _cmp_fn(_max, _x) ? _max : _x;
    }
    
    /**
     * Returns ref to _min if v is less than _min, ref to _max if _max is less than _v, otherwise ref to _v.
     * @param _x      Value to clamp.
     * @param _min    Low boundary.
     * @param _max    High boundary.
     */
    template <class T>
    constexpr const T& clamp(const T& _x, const T& _min, const T& _max)
    {
        sws_assert( !_cmp_fn(_max, _min) );
        return clamp(_x, _min, _max, std::less<>());
    }
    
    /**
     * Returns ref to _x if _x is less than _y, otherwise ref to _y.
     */
    template <typename T>
    constexpr const T& min(const T& _x, const T& _y) noexcept
    {
        return _x < _y ? _x : _y;
    }
    
    /**
     * Returns _x if _x is less than _y, otherwise _y.
     */
    inline f32 fmin(f32 _x, f32 _y) noexcept
    {
        const __m128 result = _mm_min_ss(_mm_set_ss(_x), _mm_set_ss(_y));
        return _mm_cvtss_f32(result);
    }
    
    /**
     * Returns _x if _x is less than _y, otherwise _y.
     */
    inline f64 fmin(f64 _x, f64 _y) noexcept
    {
        const __m128d result = _mm_min_sd(_mm_set_sd(_x), _mm_set_sd(_y));
        return _mm_cvtsd_f64(result);
    }
    
    /**
     * Returns ref to _x if _x is greater than _y, otherwise ref to _y.
     */
    template <typename T>
    constexpr const T& max(const T& _x, const T& _y) noexcept
    {
        return _x > _y ? _x : _y;
    }
    
    /**
     * Returns _x if _x is greater than _y, otherwise _y.
     */
    inline f32 fmax(f32 _x, f32 _y) noexcept
    {
        const __m128 result = _mm_max_ss(_mm_set_ss(_x), _mm_set_ss(_y));
        return _mm_cvtss_f32(result);
    }
    
    /**
     * Returns _x if _x is greater than _y, otherwise _y.
     */
    inline f64 fmax(f64 _x, f64 _y) noexcept
    {
        const __m128d result = _mm_max_sd(_mm_set_sd(_x), _mm_set_sd(_y));
        return _mm_cvtsd_f64(result);
    }
    
    /**
     * Returns a linear interpolation (blending) between _x and _y by _t.
     */
    template <class T, class S>
    constexpr T lerp(const T& _x, const T& _y, S _t)
    {
        return static_cast<T>(_x + _t * (_y - _x));
    }
    
    /**
     * Maps _x in range [_in_min, _in_max] to range [_out_min, _out_max].
     * @param _x       Value to be mapped.
     * @param _in_min  Low input boundary.
     * @param _in_max  High input boundary.
     * @param _out_min Low output boundary.
     * @param _out_max High output boundary.
     * @returns _x in range [_out_min, _out_max]
     */
    template <class TX, class TY>
    constexpr TY map(const TX& _x, const TX& _in_min, const TX& _in_max, const TY& _out_min, const TY& _out_max)
    {
        if (_in_min == _in_max)
            return _out_min;

        return _out_min + static_cast<TY>(_x - _in_min) * (_out_max - _out_min) / static_cast<TY>(_in_max - _in_min);
    }
    
    /**
     * Computes the ratio of _x.
     * @param _x   Value in range [_min, _max].
     * @param _min Low boundary.
     * @param _max High boundary.
     * @returns _x between [0, 1]
     */
    template <typename T>
    constexpr T ratio(const T& _x, const T& _min, const T& _max)
    {
        return (_x - _min) / (_max - _min);
    }
    
    /**
     * Returns whether or not _x is almost equal to _x within _eps range.
     */
    template <typename T>
    constexpr b32 are_almost_equal(const T& _x, const T& _y, T _eps = std::numeric_limits<T>::epsilon())
    {
        return math::abs(_x - _y) <= _eps;
    }
    
    /**
     * Returns whether or not _x is almost null within _eps range.
     */
    template <typename T>
    constexpr b32 is_almost_null(T _x, T _eps = std::numeric_limits<T>::epsilon())
    {
        return math::abs(_x) <= _eps;
    }
    
    /**
     * Returns _rad in range [-Pi, Pi].
     * @param _rad Angle in radians.
     */
    template <class T>
    constexpr T normalize_angle_Pi(const T& _rad)
    {
        return fmod(_rad, sign(_rad) * Pi<T>);
    }
    
    /**
     * Returns _rad in range [0, Pi2].
     * @param _rad Angle in radians.
     */
    template <class T>
    constexpr T normalize_angle_Pi2(const T& _rad)
    {
        return fmod(_rad, Pi2<T>);
    }
    
    /**
     * Returns _current biased by _offset towards _target.
     */
    template <class T>
    constexpr T move_towards(T _current, T _target, T _offset)
    {
        const f32 delta = _target - _current;
        if (abs(delta) <= _offset)
            return _target;

        return _current + sign(delta) * _offset;
    }
    
    /**
     * Returns _current angle biased by _offset towards _target angle (in radians).
     */
    template <class T>
    constexpr T move_towards_angle(T _current, T _target, T _offset)
    {
        _current = normalize_angle_Pi2(_current);
        _target  = normalize_angle_Pi2(_target);

        const f32 delta = _target - _current;
        if (Pi<T> + abs(delta) <= _offset)
            return _target;

        return normalize_angle_Pi2(_current + sign(delta) * _offset);
    }

    /**
     * Converts _deg in degrees to radians.
     */
    template <typename T>
    constexpr T to_radians(T _deg)
    {
        return (_deg * (math::Pi<f32> / 180.f));
    }
    
    /**
     * Converts _rad in radians to degrees.
     */
    template <typename T>
    constexpr T to_degrees(T _rad)
    {
        return (_rad * (180.f / math::Pi<f32>));
    }

    template <typename T>
    using return_t = typename std::conditional<std::is_integral<T>::value, f64, T>::type;

    template <class T>
    using GCLIM = std::numeric_limits<T>;

    namespace internal
    {
        template <typename T>
        constexpr T tan_cf_recur(const T xx, const int depth, const int max_depth)
        {
            return depth < max_depth ? T(2 * depth - 1) - xx / tan_cf_recur(xx, depth + 1, max_depth) : T(2 * depth - 1);
        }

        template <typename T>
        constexpr T tan_cf_main(const T x)
        {
            return x > T(1) ? x / tan_cf_recur(x * x, 1, 35) : x / tan_cf_recur(x * x, 1, 25);
        }

        template <typename T>
        constexpr T tan_int(const T x)
        {
            // tan(x) = tan(x + pi)
            return x > Pi<T> ? (tan_int(x - T(Pi<T>) * int(x / T(Pi<T>)))) : tan_cf_main(x);
        }

        template <typename T>
        constexpr T tan_check(const T x)
        {
            // indistinguishable from zero 
            return GCLIM<T>::epsilon() > Abs(x) ? T(0) : (x < T(0) ? -tan_int(-x) : tan_int(x));
        }
    }

    template <typename T>
    constexpr return_t<T> tan(const T& _x)
    {
        return tan_check<return_t<T>>(_x);
    }

    template <typename T>
    constexpr T acos(const T& _x)
    {
        return std::acos(_x);
    }

    template <typename T>
    constexpr T atan2(const T& _x)
    {
        return std::atan2(_x);
    }
}
