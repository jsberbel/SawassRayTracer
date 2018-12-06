#pragma once

#include "core/math/v3.h"
#include "core/utils.h"

#include <array>

class Perlin
{
public:
    static inline f32 noise(const fv3& _p);
    static constexpr f32 turbulence(const fv3& _p, s32 _depth = 7);

private:
    static constexpr usize k_perm_size = 256u;
    static constexpr usize k_perm_mask = k_perm_size - 1u;
    static constexpr usize k_cube_size = 2u;
    
    static constexpr f32 interpolate(fv3 (&_c)[k_cube_size][k_cube_size][k_cube_size], f32 _u, f32 _v, f32 _w);

    static constexpr void permute(std::array<sv3, k_perm_size>& _arr);
    static inline std::array<sv3, k_perm_size> generate_permutations();
    static inline std::array<fv3, k_perm_size> generate_rand_vectors();

private:
    static inline std::array<sv3, k_perm_size> s_perms = generate_permutations();
    static inline std::array<fv3, k_perm_size> s_rvecs = generate_rand_vectors();
};

inline f32 Perlin::noise(const fv3& _p)
{
    const s32 i = s32(math::floor(_p.x));
    const s32 j = s32(math::floor(_p.y));
    const s32 k = s32(math::floor(_p.z));

    const f32 xf = _p.x - i;
    const f32 yf = _p.y - j;
    const f32 zf = _p.z - k;

    fv3 cube[k_cube_size][k_cube_size][k_cube_size] {};

    for (usize di = 0; di < k_cube_size; ++di)
    {
        for (usize dj = 0; dj < k_cube_size; ++dj)
        {
            for (usize dk = 0; dk < k_cube_size; ++dk)
            {
                const s32 perm_x = s_perms[(i + di) & k_perm_mask].x;
                const s32 perm_y = s_perms[(j + dj) & k_perm_mask].y;
                const s32 perm_z = s_perms[(k + dk) & k_perm_mask].z;
                cube[di][dj][dk] = s_rvecs[perm_x ^ perm_y ^ perm_z];
            }
        }
    }

    const f32 result = interpolate(cube, xf, yf, zf);
    return (result + 1.f) * 0.5f;
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

inline constexpr f32 Perlin::interpolate(fv3 (&_c)[k_cube_size][k_cube_size][k_cube_size], f32 _u, f32 _v, f32 _w)
{
    const f32 uu = math::smoothstep(_u);
    const f32 vv = math::smoothstep(_v);
    const f32 ww = math::smoothstep(_w);

    f32 accum = 0.f;
    for (usize i = 0; i < k_cube_size; ++i)
    {
        for (usize j = 0; j < k_cube_size; ++j)
        {
            for (usize k = 0; k < k_cube_size; ++k)
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

constexpr void Perlin::permute(std::array<sv3, k_perm_size>& _arr)
{
    for (usize i = k_perm_size-1; i > 0; --i)
    {
        const s32 target_x = s32(util::drand_01()*(i+1));
        const s32 target_y = s32(util::drand_01()*(i+1));
        const s32 target_z = s32(util::drand_01()*(i+1));
        std::swap(_arr[i].x, _arr[target_x].x);
        std::swap(_arr[i].y, _arr[target_y].y);
        std::swap(_arr[i].z, _arr[target_z].z);
    }
}

inline std::array<sv3, Perlin::k_perm_size> Perlin::generate_permutations()
{
    std::array<sv3, k_perm_size> arr;
    for (s32 i = 0; i < k_perm_size; ++i)
        arr[i].set(i);
    permute(arr);
    return std::move(arr);
}

inline std::array<fv3, Perlin::k_perm_size> Perlin::generate_rand_vectors()
{
    std::array<fv3, k_perm_size> gradients;
    const auto rand_range = []() constexpr { return 2.f*util::frand_01() - 1.f; };
    for (usize i = 0; i < k_perm_size; ++i)
        gradients[i] = fv3(rand_range(), rand_range(), rand_range()).get_normalized();
    return std::move(gradients);
}
