#pragma once

#include "Vec3.h"

namespace Math
{
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
    constexpr Type Blend(Type start, Type end, F32 t)
    {
        return (1.f - t) * start + t * end;
    }
}