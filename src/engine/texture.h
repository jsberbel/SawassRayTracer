#pragma once

#include "core/math/v3.h"
#include "engine/perlin.h"

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

private:
    Texture* m_odd = nullptr;
    Texture* m_even = nullptr;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture() = default;
    fv3 value(f32 u, f32 v, const fv3& p) const override;
};

constexpr CheckerTexture::CheckerTexture(Texture* _odd, Texture* _even)
    : m_odd(_odd)
    , m_even(_even) 
{}

inline CheckerTexture::CheckerTexture(CheckerTexture&& _other) noexcept
    : m_odd(std::exchange(_other.m_odd, nullptr))
    , m_even(std::exchange(_other.m_even, nullptr))
{
}

inline constexpr CheckerTexture& CheckerTexture::operator=(CheckerTexture&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        m_odd = std::exchange(_other.m_odd, nullptr);
        m_even = std::exchange(_other.m_even, nullptr);
    }
    return *this;
}

inline CheckerTexture::~CheckerTexture()
{
    util::safe_del(m_odd);
    util::safe_del(m_even);
}

inline fv3 CheckerTexture::value(f32 _u, f32 _v, const fv3& _p) const
{
    const f32 sines = math::sin(10.f*_p.x)*math::sin(10.f*_p.y)*math::sin(10.f*_p.z);
    return (sines < 0.f) ? m_odd->value(_u, _v, _p) : m_even->value(_u, _v, _p);
}

inline fv3 NoiseTexture::value(f32 u, f32 v, const fv3& p) const
{
    return fv3(1.f) * Perlin::noise(p);
}
