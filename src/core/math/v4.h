//// ======================================================================
//// File: v4.h
//// Revision: 1.0
//// Creation: 11/18/2018 - jsberbel
//// Notice: Copyright © 2018 by Jordi Serrano Berbel. All Rights Reserved.
//// ======================================================================
//
//#pragma once
//
//#include "core/v3.h"
//
//template <class T>
//union v4
//{
//public:
//    constexpr v4() noexcept                                       : x(0), y(0), z(0) {}
//    explicit constexpr v4(T _s) noexcept                          : x(_s), y(_s), z(_s) {}
//    explicit constexpr v4(const v2<T>& _xy, T _z = T(0)) noexcept : xy(_xy), z(_z) {}
//
//    template <typename TT>
//    constexpr v4<TT> cast() const { return v4<TT>(TT(x), TT(y), TT(z)); }
//    
//    constexpr T operator[](size_t _i) const noexcept { sws_assert(_i < 3); return (&x)[_i]; }
//    constexpr T& operator[](size_t _i) noexcept      { sws_assert(_i < 3); return (&x)[_i]; }
//
//    constexpr const v4& operator+() const noexcept;
//    constexpr v4 operator-() const noexcept;
//
//    constexpr v4& operator+=(const v4& _v) noexcept;
//    constexpr v4& operator+=(T _s) noexcept;
//    constexpr v4 operator+(const v4& _v) const noexcept;
//
//    constexpr v4& operator-=(const v4& _v) noexcept;
//    constexpr v4& operator-=(T _s) noexcept;
//    constexpr v4 operator-(const v4& _v) const noexcept;
//
//    constexpr v4& operator*=(const v4& _v) noexcept;
//    constexpr v4& operator*=(T _s) noexcept;
//    constexpr v4 operator*(const v4& _v) const noexcept;
//    constexpr v4 operator*(T _s) const noexcept;
//
//    constexpr v4& operator/=(const v4& _v) noexcept;
//    constexpr v4& operator/=(T _s) noexcept;
//    constexpr v4 operator/(const v4& _v) const noexcept;
//    constexpr v4 operator/(T _s) const noexcept;
//
//    constexpr b32 operator==(const v4& _v) const;
//    constexpr b32 operator!=(const v4& _v) const;
//    constexpr b32 operator<(const v4& _v) const;
//    constexpr b32 operator<=(const v4& _v) const;
//    constexpr b32 operator>(const v4& _v) const;
//    constexpr b32 operator>=(const v4& _v) const;
//
//    friend constexpr std::istream& operator>>(std::istream& _is, v4& _v) noexcept;
//    friend inline std::ostream& operator<<(std::ostream& _os, const v4& _v) noexcept;
//
//    static constexpr v4 x_axis()   { return v4(T(1), T(0), T(0)); }
//    static constexpr v4 y_axis()   { return v4(T(0), T(1), T(0)); }
//    static constexpr v4 z_axis()   { return v4(T(0), T(0), T(1)); }
//    static constexpr v4 right()    { return v4(T(1), T(0), T(0)); }
//    static constexpr v4 front()    { return v4(T(0), T(1), T(0)); }
//    static constexpr v4 up()       { return v4(T(0), T(0), T(1)); }
//    static constexpr v4 zero()     { return v4(T(0)); }
//    static constexpr v4 one()      { return v4(T(1)); }
//    static constexpr v4 infinity() { return v4(std::numeric_limits<T>::infinity()); }
//    static constexpr v4 nan()      { return v4(std::numeric_limits<T>::quiet_NaN()); }
//
//    constexpr b32 is_nan() const noexcept;
//    constexpr b32 is_finite() const noexcept;
//    constexpr b32 is_normalized() const noexcept;
//
//    constexpr T get_sqrlength() const noexcept;
//    constexpr T get_length() const noexcept;
//    constexpr v4 get_normalized() const noexcept;
//    constexpr v4 get_abs() const noexcept;
//    constexpr v4 get_orthogonal() const noexcept;
//    constexpr v4 get_inverse() const noexcept;
//    constexpr v4 get_floor() const noexcept;
//    constexpr v4 get_ceil() const noexcept;
//    constexpr T get_shortest_angle(const v4& _axis = right()) const noexcept;
//
//    inline void set_length(T _len) noexcept;
//    inline void normalize() noexcept;
//    inline void clear() noexcept;
//    
//    constexpr v2<T>& get_xy() { return xy; }
//    constexpr v2<T>& get_yz() { return yz; }
//
//public:
//    struct { T x, y, z; };
//    struct { T r, g, b; };
//
//private:
//    struct { v2<T> xy; T _0; };
//    struct { T _1; v2<T> yz; };
//};
//
//namespace math
//{
//    template <class T>
//    constexpr T sqrdist(const v4<T>& _a, const v4<T>& _b) noexcept
//    {
//        return (_b - _a).get_sqrlength();
//    }
//
//    template <class T>
//    constexpr T dist(const v4<T>& _a, const v4<T>& _b) noexcept
//    {
//        return (_b - _a).get_length();
//    }
//
//    template <class T>
//    constexpr T dot(const v4<T>& _a, const v4<T>& _b) noexcept
//    {
//        return _a.x*_b.x + _a.y*_b.y + _a.z*_b.z;
//    }
//    
//    template <class T>
//    constexpr T cross(const v4<T>& _a, const v4<T>& _b) noexcept
//    {
//        return v4<T>(
//                      _a.y*_b.z - _a.z*_b.y,
//                      _a.z*_b.x - _a.x*_b.z,
//                      _a.x*_b.y - _a.y*_b.x
//                    );
//    }
//}
//
//template <class T>
//constexpr const v4<T>& v4<T>::operator+() const noexcept
//{
//    return *this;
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator-() const noexcept
//{
//    return v4<T>(-x, -y, -z);
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator+=(const v4<T>& _v) noexcept
//{
//    x += _v.x;
//    y += _v.y;
//    z += _v.z;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator+=(T _s) noexcept
//{
//    x += _s;
//    y += _s;
//    z += _s;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator+(const v4<T>& _v) const noexcept
//{
//    return v4<T>(x + _v.x, y + _v.y, z + _v.z);
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator-=(const v4<T>& _v) noexcept
//{
//    x -= _v.x;
//    y -= _v.y;
//    z -= _v.z;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator-=(T _s) noexcept
//{
//    x -= _s;
//    y -= _s;
//    z -= _s;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator-(const v4<T>& _v) const noexcept
//{
//    return v4<T>(x - _v.x, y - _v.y, z - _v.z);
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator*=(const v4<T>& _v) noexcept
//{
//    x *= _v.x;
//    y *= _v.y;
//    z *= _v.z;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator*=(T _s) noexcept
//{
//    x *= _s;
//    y *= _s;
//    z *= _s;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator*(const v4<T>& _v) const noexcept
//{
//    return v4<T>(x * _v.x, y * _v.y, z * _v.z);
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator*(T _s) const noexcept
//{
//    return v4<T>(x * _s, y * _s, z * _s);
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator/=(const v4<T>& _v) noexcept
//{
//    x /= _v.x;
//    y /= _v.y;
//    z /= _v.z;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T>& v4<T>::operator/=(const T _s) noexcept
//{
//    const f32 inv_s = math::inv(_s);
//    x *= inv_s;
//    y *= inv_s;
//    z *= inv_s;
//    return *this;
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator/(const v4<T>& _v) const noexcept
//{
//    return v4<T>(x / _v.x, y / _v.y, z / _v.z);
//}
//
//template <class T>
//constexpr v4<T> v4<T>::operator/(T _s) const noexcept
//{
//    return v4<T>(x / _s, y / _s, z / _s);
//}
//
//template <class T>
//constexpr b32 v4<T>::operator==(const v4& _v) const
//{
//    return x == _v.x && y == _v.y && z == _v.z;
//}
//
//template <class T>
//constexpr b32 v4<T>::operator!=(const v4& _v) const
//{
//    return x != _v.x || y != _v.y || z != _v.z;
//}
//
//template <class T>
//constexpr b32 v4<T>::operator<(const v4& _v) const
//{
//    return x < _v.x && y < _v.y && z < _v.z;
//}
//
//template <class T>
//constexpr b32 v4<T>::operator<=(const v4& _v) const
//{
//    return x <= _v.x && y <= _v.y && z <= _v.z;
//}
//
//template <class T>
//constexpr b32 v4<T>::operator>(const v4& _v) const
//{
//    return x > _v.x && y > _v.y && z > _v.z;
//}
//
//template <class T>
//constexpr b32 v4<T>::operator>=(const v4& _v) const
//{
//    return x >= _v.x && y >= _v.y && z >= _v.z;
//}
//
//template <class T>
//constexpr std::istream& operator>>(std::istream& _is, v4<T>& _v) noexcept
//{
//    _is >> _v.x >> _v.y >> _v.z;
//    return _is;
//}
//
//template <class T>
//inline std::ostream& operator<<(std::ostream& _os, const v4<T>& _v) noexcept
//{
//    _os << _v.x << ' ' << _v.y << ' ' << _v.z;
//    return _os;
//}
//
//template <class T>
//constexpr b32 v4<T>::is_nan() const noexcept
//{
//    return math::is_nan(x) || math::is_nan(y) || math::is_nan(z);
//}
//
//template <class T>
//constexpr b32 v4<T>::is_finite() const noexcept
//{
//    return math::is_finite(x) && math::is_finite(y) && math::is_finite(z);
//}
//
//template <class T>
//constexpr b32 v4<T>::is_normalized() const noexcept
//{
//    const f32 sqrlength = get_sqrlength();
//    return math::is_almost_null(sqrlength);
//}
//
//template <class T>
//constexpr T v4<T>::get_sqrlength() const noexcept
//{
//    return x*x + y*y + z*z;
//}
//
//template <class T>
//constexpr T v4<T>::get_length() const noexcept
//{
//    return math::sqrt(get_sqrlength());
//}
//
//template <class T>
//constexpr v4<T> v4<T>::get_normalized() const noexcept
//{
//    const T inv_len = math::inv(get_length());
//    return v4<T>(x * inv_len, y * inv_len, z * inv_len);
//}
//
//template <class T>
//constexpr v4<T> v4<T>::get_abs() const noexcept
//{
//    return v4<T>(math::abs(x), math::abs(y), math::abs(z));
//}
//
//template <class T>
//constexpr v4<T> v4<T>::get_orthogonal() const noexcept
//{
//    const T len = get_length();
//    if (is_almost_equal(z, T(0)))
//        return up() * len;
//
//    const T out_x = y;
//    const T out_y = z;
//    const T out_z = -(x * out_x + y * out_y) / z;
//    v4<T> out(out_x, out_y, out_z);
//    out.set_length(len);
//    return out;
//}
//
//template <class T>
//constexpr v4<T> v4<T>::get_inverse() const noexcept
//{
//    return v4<T>(math::inv(x), math::inv(y), math::inv(z));
//}
//
//template <class T>
//constexpr v4<T> v4<T>::get_floor() const noexcept
//{
//    return v4<T>(math::floor(x), math::floor(y), math::floor(z));
//}
//
//template <class T>
//constexpr v4<T> v4<T>::get_ceil() const noexcept
//{
//    return v4<T>(math::ceil(x), math::ceil(y), math::ceil(z));
//}
//
//template <class T>
//constexpr T v4<T>::get_shortest_angle(const v4<T>& _axis) const noexcept
//{
//    const T angle = math::acos(math::dot(_axis.get_normalized(), this->get_normalized()));
//    sws_assert(angle >= T(0));
//    sws_assert(angle < math::Pi2<T>);
//    return angle;
//}
//
//template <class T>
//inline void v4<T>::set_length(T _len) noexcept
//{
//    normalize();
//    *this *= _len;
//}
//
//template <class T>
//inline void v4<T>::normalize() noexcept
//{
//    const T len = get_length();
//    if (len > std::numeric_limits<T>::epsilon())
//        *this /= len;
//    else
//        clear();
//}
//
//template <class T>
//inline void v4<T>::clear() noexcept
//{
//    x = y = z = T(0);
//}
//
//template <class T>
//inline v4<T> operator*(T _s, const v4<T>& _v)
//{
//    return _v.operator*(_s);
//}
//
//using fv4 = v4<f32>;
//using dv4 = v4<f64>;
//using uv4 = v4<u32>;
//using sv4 = v4<s32>;
//
//union v4
//{
//public:
//    v4() noexcept : sse(_mm_setzero_ps())
//    {
//    }
//
//    v4(f32 e) noexcept : sse(_mm_set_ps1(e))
//    {
//    }
//
//    v4(f32 x, f32 y, f32 z, f32 w) noexcept : sse(_mm_setr_ps(x, y, z, w))
//    {
//    }
//
//    constexpr v4(const v4& v) noexcept = default;
//    constexpr v4(v4&& v) noexcept = default;
//
//    constexpr v4& operator=(const v4& v) noexcept;
//    constexpr v4& operator=(v4&& v) noexcept;
//
//    constexpr f32 operator[](size_t i) const noexcept
//    {
//        sws_assert(i < 5);
//        return (&x)[i];
//    }
//
//    constexpr f32& operator[](size_t i) noexcept
//    {
//        sws_assert(i < 5);
//        return (&x)[i];
//    }
//
//    constexpr f32x2& get_xy() { return xy; }
//    constexpr f32x2& get_yz() { return yz; }
//    constexpr f32x2& get_zw() { return zw; }
//    constexpr v4& get_xyz() { return xyz; }
//    constexpr v4& get_yzw() { return yzw; }
//
//public:
//    struct
//    {
//        f32 x, y, z, w;
//    };
//
//    struct
//    {
//        f32 r, g, b, a;
//    };
//
//private:
//    struct
//    {
//        f32x2 xy;
//        f32 _0;
//        f32 _1;
//    };
//
//    struct
//    {
//        f32 _2;
//        f32x2 yz;
//        f32 _3;
//    };
//
//    struct
//    {
//        f32 _4;
//        f32 _5;
//        f32x2 zw;
//    };
//
//    struct
//    {
//        v4 xyz;
//        f32 _6;
//    };
//
//    struct
//    {
//        f32 _7;
//        v4 yzw;
//    };
//
//    __m128 sse;
//};
//
//constexpr v4& v4::operator=(const v4& v) noexcept
//{
//    if (this != std::addressof(v))
//    {
//        sse = v.sse;
//    }
//    return *this;
//}
//
//constexpr v4& v4::operator=(v4&& v) noexcept
//{
//    if (this != std::addressof(v))
//    {
//        sse = std::move(v.sse);
//    }
//    return *this;
//}
//
//static_assert(sizeof(f32x2) == 4 * 2);
//static_assert(sizeof(v4) == 4 * 3);
//static_assert(sizeof(v4) == 4 * 4);
//
//template <typename Type>
//class vec3
//{
//public:
//    constexpr vec3() noexcept;
//    constexpr vec3(Type t) noexcept;
//    constexpr vec3(Type x, Type y, Type z) noexcept;
//
//    constexpr vec3(const vec3& v) noexcept;
//    constexpr vec3(vec3&& v) noexcept;
//
//    constexpr vec3& operator=(const vec3& v) noexcept;
//    constexpr vec3& operator=(vec3&& v) noexcept;
//
//    constexpr const vec3& operator+() const noexcept;
//    constexpr vec3 operator-() const noexcept;
//    constexpr Type operator[](size i) const noexcept;
//    constexpr Type& operator[](size i) noexcept;
//
//    constexpr vec3& operator+=(const vec3& v) noexcept;
//    constexpr vec3& operator-=(const vec3& v) noexcept;
//    constexpr vec3& operator*=(const vec3& v) noexcept;
//    constexpr vec3& operator/=(const vec3& v) noexcept;
//    constexpr vec3& operator*=(Type t) noexcept;
//    constexpr vec3& operator/=(Type t) noexcept;
//
//    friend constexpr std::istream& operator>>(std::istream& is, vec3& v) noexcept;
//    friend constexpr std::ostream& operator<<(std::ostream& os, const vec3& v) noexcept;
//
//    constexpr Type Length() const noexcept;
//    constexpr Type SquaredLength() const noexcept;
//    constexpr vec3 Normalize() const noexcept;
//
//public:
//    union
//    {
//        struct
//        {
//            Type X, Y, Z;
//        };
//
//        struct
//        {
//            Type R, G, B;
//        };
//
//        struct
//        {
//            Type S, T, P;
//        };
//    };
//};
//
//template <typename Type>
//constexpr vec3<Type>::vec3() noexcept = default;
//
//template <typename Type>
//constexpr vec3<Type>::vec3(Type t) noexcept
//    : X(t)
//    , Y(t)
//    , Z(t)
//{
//}
//
//template <typename Type>
//constexpr vec3<Type>::vec3(Type x, Type y, Type z) noexcept
//    : X(x)
//    , Y(y)
//    , Z(z)
//{
//}
//
//template <typename Type>
//constexpr vec3<Type>::vec3(const vec3& v) noexcept
//    : X(v.X)
//    , Y(v.Y)
//    , Z(v.Z)
//{
//}
//
//template <typename Type>
//constexpr vec3<Type>::vec3(vec3&& v) noexcept
//    : X(std::move(v.X))
//    , Y(std::move(v.Y))
//    , Z(std::move(v.Z))
//{
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator=(const vec3& v) noexcept
//{
//    if (this != &v)
//    {
//        X = v.X;
//        Y = v.Y;
//        Z = v.Z;
//    }
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator=(vec3<Type>&& v) noexcept
//{
//    if (this != &v)
//    {
//        X = std::exchange(v.X, 0.f);
//        Y = std::exchange(v.Y, 0.f);
//        Z = std::exchange(v.Z, 0.f);
//    }
//    return *this;
//}
//
//template <typename Type>
//constexpr const vec3<Type>& vec3<Type>::operator+() const noexcept
//{
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type> vec3<Type>::operator-() const noexcept
//{
//    return vec3<Type>(-X, -Y, -Z);
//}
//
//template <typename Type>
//constexpr Type vec3<Type>::operator[](size i) const noexcept
//{
//    sws_assert(i < 3);
//    return (&X)[i];
//}
//
//template <typename Type>
//constexpr Type& vec3<Type>::operator[](size i) noexcept
//{
//    sws_assert(i < 3);
//    return (&X)[i];
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator+=(const vec3<Type>& v) noexcept
//{
//    X += v.X;
//    Y += v.Y;
//    Z += v.Z;
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator-=(const vec3<Type>& v) noexcept
//{
//    X -= v.X;
//    Y -= v.Y;
//    Z -= v.Z;
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator*=(const vec3<Type>& v) noexcept
//{
//    X *= v.X;
//    Y *= v.Y;
//    Z *= v.Z;
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator/=(const vec3<Type>& v) noexcept
//{
//    X /= v.X;
//    Y /= v.Y;
//    Z /= v.Z;
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator*=(const Type t) noexcept
//{
//    X *= t;
//    Y *= t;
//    Z *= t;
//    return *this;
//}
//
//template <typename Type>
//constexpr vec3<Type>& vec3<Type>::operator/=(const Type t) noexcept
//{
//    const float k = 1.f / t;
//    X *= k;
//    Y *= k;
//    Z *= k;
//    return *this;
//}
//
//template <typename Type>
//constexpr std::istream& operator>>(std::istream& is, vec3<Type>& v) noexcept
//{
//    is >> v.X >> v.Y >> v.Z;
//    return is;
//}
//
//template <typename Type>
//std::ostream& operator<<(std::ostream& os, const vec3<Type>& v) noexcept
//{
//    os << v.X << " " << v.Y << " " << v.Z;
//    return os;
//}
//
//template <typename Type>
//constexpr Type vec3<Type>::Length() const noexcept
//{
//    return std::sqrt(SquaredLength());
//}
//
//template <typename Type>
//constexpr Type vec3<Type>::SquaredLength() const noexcept
//{
//    return X * X + Y * Y + Z * Z;
//}
//
//template <typename Type>
//constexpr vec3<Type> vec3<Type>::Normalize() const noexcept
//{
//    const Type k = 1.f / Length();
//    return vec3<Type>(X * k, Y * k, Z * k);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator+(const vec3<Type>& lhs, const vec3<Type>& rhs) noexcept
//{
//    return vec3<Type>(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator-(const vec3<Type>& lhs, const vec3<Type>& rhs) noexcept
//{
//    return vec3<Type>(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator*(const vec3<Type>& lhs, const vec3<Type>& rhs) noexcept
//{
//    return vec3<Type>(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator/(const vec3<Type>& lhs, const vec3<Type>& rhs) noexcept
//{
//    return vec3<Type>(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator*(const Type& t, const vec3<Type>& v) noexcept
//{
//    return vec3<Type>(t * v.X, t * v.Y, t * v.Z);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator*(const vec3<Type>& v, Type t) noexcept
//{
//    return vec3<Type>(v.X * t, v.Y * t, v.Z * t);
//}
//
//template <typename Type>
//constexpr vec3<Type> operator/(const vec3<Type>& v, Type t) noexcept
//{
//    return vec3<Type>(v.X / t, v.Y / t, v.Z / t);
//}
//
//using BVec3 = vec3<bool>;
//
//using UVec3 = vec3<u32>;
//using SVec3 = vec3<s32>;
//
//using fvec3 = vec3<f32>;
//using DVec3 = vec3<f64>;
//
//using fv3 = vec3<byte>;
