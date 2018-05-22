#pragma once

#include "Types.h"

template <typename Type>
struct Limits
{
    static constexpr Type Min() noexcept { return Type(); }
    static constexpr Type Max() noexcept { return Type(); }
};

template<>
struct Limits <U8>
{
    using Type = U8;
    static constexpr Type Min() noexcept { return 0U; }
    static constexpr Type Max() noexcept { return 0xffU; }
};

template<>
struct Limits <U16>
{
    using Type = U16;
    static constexpr Type Min() noexcept { return 0U; }
    static constexpr Type Max() noexcept { return 0xffU; }
};

template<>
struct Limits <U32>
{
    using Type = U32;
    static constexpr Type Min() noexcept { return 0U; }
    static constexpr Type Max() noexcept { return 0xffffffffU; }
};

template<>
struct Limits <U64>
{
    using Type = U64;
    static constexpr Type Min() noexcept { return 0ULL; }
    static constexpr Type Max() noexcept { return 0xffffffffffffffffULL; }
};

template<>
struct Limits <S8>
{
    using Type = S8;
    static constexpr Type Min() noexcept { return -128; }
    static constexpr Type Max() noexcept { return 127; }
};

template<>
struct Limits <S16>
{
    using Type = S16;
    static constexpr Type Min() noexcept { return -32768; }
    static constexpr Type Max() noexcept { return 32767; }
};

template<>
struct Limits <S32>
{
    using Type = S32;
    static constexpr Type Min() noexcept { return -2147483647 - 1; }
    static constexpr Type Max() noexcept { return 2147483647; }
};

template<>
struct Limits <S64>
{
    using Type = S64;
    static constexpr Type Min() noexcept { return -9223372036854775807LL - 1; }
    static constexpr Type Max() noexcept { return 9223372036854775807LL; }
};

template<>
struct Limits <F32>
{
    using Type = F32;
    static constexpr Type Min() noexcept { return 1.175494351e-38F; }
    static constexpr Type Max() noexcept { return 3.402823466e+38F; }
};

template<>
struct Limits <F64>
{
    using Type = F64;
    static constexpr Type Min() noexcept { return 2.2250738585072014e-308; }
    static constexpr Type Max() noexcept { return 1.7976931348623158e+308; }
};

template<>
struct Limits <UTF8>
{
    using Type = UTF8;
    static constexpr Type Min() noexcept { return -128; }
    static constexpr Type Max() noexcept { return 127; }
};

template<>
struct Limits <UTF16>
{
    using Type = UTF16;
    static constexpr Type Min() noexcept { return 0; }
    static constexpr Type Max() noexcept { return 0xffff; }
};

template<>
struct Limits <UTF32>
{
    using Type = UTF32;
    static constexpr Type Min() noexcept { return 0; }
    static constexpr Type Max() noexcept { return 0xffffffff; }
};