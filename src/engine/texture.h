#pragma once

#include "core/math/v3.h"
#include "engine/perlin.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture
{
public:
    virtual inline ~Texture() = default;
    virtual inline fv3 value(const V2f& _uv, const fv3& _p) const = 0;
};

class ConstTexture : public Texture
{
public:
    constexpr ConstTexture() = default;
    constexpr ConstTexture(const fv3& _c) : color(_c) {}
    virtual inline ~ConstTexture() = default;

    inline fv3 value(const V2f& _uv, const fv3& _p) const override { return color; }

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

    inline fv3 value(const V2f& _uv, const fv3& _p) const override;

private:
    Texture* m_odd = nullptr;
    Texture* m_even = nullptr;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(f32 _scale = 1.f);

    fv3 value(const V2f& _uv, const fv3& _p) const override;

public:
    f32 scale;
};

class ImageTexture : public Texture
{
    NON_COPYABLE(ImageTexture);
public:
    inline ImageTexture(const char* _filepath);
    inline ImageTexture(const fs::path& _filepath);
    inline ImageTexture(ImageTexture&& _other) noexcept;
    constexpr ImageTexture& operator=(ImageTexture&& _other) noexcept;
    virtual inline ~ImageTexture();

    fv3 value(const V2f& _uv, const fv3& _p) const override;

private:
    uchar* m_data = nullptr;
    s32 m_width;
    s32 m_height;
    s32 m_bpp;
};

// CheckerTexture //

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

inline fv3 CheckerTexture::value(const V2f& _uv, const fv3& _p) const
{
    const f32 sines = math::sin(10.f*_p.x)*math::sin(10.f*_p.y)*math::sin(10.f*_p.z);
    return (sines < 0.f) ? m_odd->value(_uv, _p) : m_even->value(_uv, _p);
}

// NoiseTexture //

inline NoiseTexture::NoiseTexture(f32 _scale)
    : scale(_scale)
{
}

inline fv3 NoiseTexture::value(const V2f& _uv, const fv3& _p) const
{
    //return fv3(1.f) * (Perlin::noise(scale*_p));
    //return fv3(1.f) * 0.5f * (1.f + math::sin(scale*_p.x + 5.f*Perlin::turbulence(scale*_p)));
    //return fv3(1.f) * 0.5f * (1.f + math::sin(scale*_p.z + 10.f*Perlin::turbulence(_p)));
    //return fv3(1.f) * (Perlin::cloud(_p*scale));
    //return fv3(1.f) * (Perlin::wood(_p*scale));
    return fv3(1.f) * (Perlin::marble(_p*scale));
}

// ImageTexture //

inline ImageTexture::ImageTexture(const char* _filepath)
    : m_data(stbi_load(_filepath, &m_width, &m_height, &m_bpp, 0))
{
    sws_assert(m_data);
}

inline ImageTexture::ImageTexture(const fs::path& _filepath)
    : ImageTexture(_filepath.string().c_str())
{
}

inline ImageTexture::ImageTexture(ImageTexture&& _other) noexcept
    : m_data(std::exchange(_other.m_data, nullptr))
    , m_width(_other.m_width)
    , m_height(_other.m_height)
    , m_bpp(_other.m_bpp)
{
}

inline constexpr ImageTexture& ImageTexture::operator=(ImageTexture&& _other) noexcept
{
    if (this != std::addressof(_other))
    {
        m_data = std::exchange(_other.m_data, nullptr);
        m_width = _other.m_width;
        m_height = _other.m_height;
        m_bpp = _other.m_bpp;
    }
    return *this;
}

inline ImageTexture::~ImageTexture()
{
    util::safe_del(m_data);
}

inline fv3 ImageTexture::value(const V2f& _uv, const fv3& _p) const
{
    s32 i = s32(m_width * _uv.u);
    s32 j = s32(m_height * (1.f-_uv.v) - 0.001f);
    i = math::clamp(i, 0, m_width-1);
    j = math::clamp(j, 0, m_height-1);
    const f32 r = s32(m_data[3*i + 3*m_width*j+0]) / 255.f;
    const f32 g = s32(m_data[3*i + 3*m_width*j+1]) / 255.f;
    const f32 b = s32(m_data[3*i + 3*m_width*j+2]) / 255.f;
    return fv3(r, g, b);
}
