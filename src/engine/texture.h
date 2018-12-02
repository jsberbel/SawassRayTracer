#pragma once

#include "core/math/v3.h"

class Texture
{
public:
    virtual inline ~Texture() = default;
    virtual inline fv3 value(f32 _u, f32 _v, const fv3& _p) const = 0;
};

class ConstTexture : public Texture
{
public:
    constexpr ConstTexture() = default;
    constexpr ConstTexture(const fv3& _c) : color(_c) {}
    virtual inline ~ConstTexture() = default;
    inline fv3 value(f32 _u, f32 _v, const fv3& _p) const override { return color; }

public:
    fv3 color;
};

class CheckerTexture : public Texture
{
public:
    constexpr CheckerTexture() = default;
    constexpr explicit CheckerTexture(Texture* _odd, Texture* _even);
    constexpr CheckerTexture(const CheckerTexture&) noexcept = delete;
    constexpr CheckerTexture& operator=(const CheckerTexture&) noexcept = delete;
    inline CheckerTexture(CheckerTexture&& _other) noexcept;
    constexpr CheckerTexture& operator=(CheckerTexture&& _other) noexcept;
    virtual inline ~CheckerTexture();

    inline fv3 value(f32 _u, f32 _v, const fv3& _p) const override;

public:
    Texture* odd = nullptr;
    Texture* even = nullptr;
};

constexpr CheckerTexture::CheckerTexture(Texture* _odd, Texture* _even)
    : odd(_odd)
    , even(_even) 
{}

inline CheckerTexture::CheckerTexture(CheckerTexture&& _other) noexcept
    : odd(std::exchange(_other.odd, nullptr))
    , even(std::exchange(_other.even, nullptr))
{
}

inline constexpr CheckerTexture& CheckerTexture::operator=(CheckerTexture&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        odd = std::exchange(_other.odd, nullptr);
        even = std::exchange(_other.even, nullptr);
    }
    return *this;
}

inline CheckerTexture::~CheckerTexture()
{
    util::safe_del(odd);
    util::safe_del(even);
}

inline fv3 CheckerTexture::value(f32 _u, f32 _v, const fv3& _p) const
{
    const f32 sines = math::sin(10.f*_p.x)*math::sin(10.f*_p.y)*math::sin(10.f*_p.z);
    return (sines < 0.f) ? odd->value(_u, _v, _p) : even->value(_u, _v, _p);
}