#pragma once

#include "core/math/v3.h"
#include "core/utils.h"

#include <array>

class Perlin
{
public:
    static constexpr f32 noise(const fv3& _p);
    static constexpr f32 turbulence(const fv3& _p, s32 _depth = 7);

private:
    static constexpr usz k_size = 256u;

    static constexpr void permute(std::array<sv3, k_size>& _arr, s32 _n);
    static inline std::array<fv3, k_size> generate_frand();
    static inline std::array<sv3, k_size> generate_perm();
    
    static constexpr f32 lerp(fv3 _c[2][2][2], f32 _u, f32 _v, f32 _w);

private:
    static inline std::array<fv3, k_size> s_frand = generate_frand();
    static inline std::array<sv3, k_size> s_perm  = generate_perm();
};

constexpr f32 Perlin::noise(const fv3& _p)
{
    const f32 u = _p.x - math::floor(_p.x);
    const f32 v = _p.y - math::floor(_p.y);
    const f32 w = _p.z - math::floor(_p.z);
    const s32 i = s32(math::floor(_p.x));
    const s32 j = s32(math::floor(_p.y));
    const s32 k = s32(math::floor(_p.z));
    fv3 c[2][2][2] {};
    for (usz di = 0; di < 2; ++di)
    {
        for (usz dj = 0; dj < 2; ++dj)
        {
            for (usz dk = 0; dk < 2; ++dk)
            {
                const s32 perm_x = s_perm[i + di].x & 255;
                const s32 perm_y = s_perm[j + dj].y & 255;
                const s32 perm_z = s_perm[k + dk].z & 255;
                c[di][dj][dk] = s_frand[perm_x ^ perm_y ^ perm_z];
            }
        }
    }
    return lerp(c, u, v, w);
}

inline constexpr f32 Perlin::turbulence(const fv3& _p, s32 _depth)
{
    f32 accum = 0.f;
    fv3 tmp_p = _p;
    f32 weight = 1.f;
    for (s32 i = 0; i < _depth; ++i)
    {
        accum += weight * noise(tmp_p);
        weight *= 0.5f;
        tmp_p *= 2.f;
    }
    return math::abs(accum);
}

constexpr void Perlin::permute(std::array<sv3, k_size>& _arr, s32 _n)
{
    for (usz i = _n-1; i > 0; --i)
    {
        const s32 target_x = s32(util::drand_01()*(i+1));
        const s32 target_y = s32(util::drand_01()*(i+1));
        const s32 target_z = s32(util::drand_01()*(i+1));
        std::swap(_arr[i].x, _arr[target_x].x);
        std::swap(_arr[i].y, _arr[target_y].y);
        std::swap(_arr[i].z, _arr[target_z].z);
    }
}

inline std::array<fv3, Perlin::k_size> Perlin::generate_frand()
{
    std::array<fv3, k_size> arr;
    for (usz i = 0; i < k_size; ++i)
        arr[i] = fv3(-1.f + 2.f*util::frand_01(),
                     -1.f + 2.f*util::frand_01(),
                     -1.f + 2.f*util::frand_01()).get_normalized();
    return std::move(arr);
}

inline std::array<sv3, Perlin::k_size> Perlin::generate_perm()
{
    std::array<sv3, k_size> arr;
    for (s32 i = 0; i < 256; ++i)
        arr[i].set(i);
    permute(arr, 256);
    return std::move(arr);
}

inline constexpr f32 Perlin::lerp(fv3 _c[2][2][2], f32 _u, f32 _v, f32 _w)
{
    const f32 uu = math::smoothstep(_u);
    const f32 vv = math::smoothstep(_v);
    const f32 ww = math::smoothstep(_w);
    f32 accum = 0.f;
    for (usz i = 0; i < 2; ++i)
    {
        for (usz j = 0; j < 2; ++j)
        {
            for (usz k = 0; k < 2; ++k)
            {
                const fv3 weight(_u-i, _v-j, _w-k);
                accum += (i*uu + (1-i)*(1-uu)) *
                         (j*vv + (1-j)*(1-vv)) *
                         (k*ww + (1-k)*(1-ww)) * math::dot(_c[i][j][k], weight);
            }
        }
    }
    return accum;
}
